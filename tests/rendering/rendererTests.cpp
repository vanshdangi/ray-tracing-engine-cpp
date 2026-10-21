#include <gtest/gtest.h>
#include <rendering/renderer.hpp>
#include <render_stats.hpp>
#include <world/objects/sphere.hpp>

#include <array>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

Camera makeCamera(int width, int height) {
    return Camera(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 1.0f),
        90.0f,
        static_cast<float>(width) / static_cast<float>(height),
        static_cast<float>(width),
        static_cast<float>(height)
    );
}

Color expectedBackgroundPixel(const Camera& camera, int x, int y) {
    constexpr std::array<float, 4> offsets{
        0.125f, 0.375f, 0.625f, 0.875f
    };
    Color color;

    for (float sampleY : offsets) {
        for (float sampleX : offsets) {
            color += [&] {
                const Ray ray = camera.generateRay(x, y, sampleX, sampleY);
                const float directionY = ray.direction.y;
                const Color top = Color::fromSrgb(0.275f, 0.588f, 0.863f);
                const Color horizon = Color::fromSrgb(0.824f, 0.922f, 0.961f);
                const Color ground = Color::fromSrgb(0.373f, 0.353f, 0.333f);

                if (directionY > 0.0f) {
                    return horizon * (1.0f - directionY) + top * directionY;
                }
                const float amount = -directionY;
                return horizon * (1.0f - amount) + ground * amount;
            }();
        }
    }

    return color / 16.0f;
}

void expectColorNear(const Color& actual, const Color& expected) {
    EXPECT_NEAR(actual.r, expected.r, 1e-5f);
    EXPECT_NEAR(actual.g, expected.g, 1e-5f);
    EXPECT_NEAR(actual.b, expected.b, 1e-5f);
}

} // namespace

TEST(RendererTest, BackgroundUsesTopHorizonAndGroundColors) {
    Camera camera = makeCamera(1, 1);
    Image image(1, 1);
    Scene scene;
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 1.0f, Color(1.0f, 1.0f, 1.0f));
    const Renderer renderer(camera, image, scene, light, 0.15f);

    const Color top = Color::fromSrgb(0.275f, 0.588f, 0.863f);
    const Color horizon = Color::fromSrgb(0.824f, 0.922f, 0.961f);
    const Color ground = Color::fromSrgb(0.373f, 0.353f, 0.333f);

    expectColorNear(renderer.calculateBackground(
        Ray{Point3(), Vec3(0.0f, 1.0f, 0.0f)}), top);
    expectColorNear(renderer.calculateBackground(
        Ray{Point3(), Vec3(1.0f, 0.0f, 0.0f)}), horizon);
    expectColorNear(renderer.calculateBackground(
        Ray{Point3(), Vec3(0.0f, -1.0f, 0.0f)}), ground);
}

TEST(RendererTest, FindsNearestSceneIntersection) {
    Camera camera = makeCamera(1, 1);
    Image image(1, 1);
    Scene scene;
    const Transform nearTransform(
        Point3(0.0f, 0.0f, 3.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
    const Transform farTransform(
        Point3(0.0f, 0.0f, 5.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
    scene.addObject(std::make_unique<Sphere>(nearTransform, 1.0f, Material()));
    scene.addObject(std::make_unique<Sphere>(farTransform, 1.0f, Material()));
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 1.0f, Color(1.0f, 1.0f, 1.0f));
    const Renderer renderer(camera, image, scene, light, 0.15f);

    const auto hit = renderer.findClosestHit(
        Ray{Point3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)});

    ASSERT_TRUE(hit.has_value());
    EXPECT_FLOAT_EQ(hit->t, 2.0f);
    EXPECT_EQ(hit->intersection.object, scene.getObjects()[0].get());
}

TEST(RendererTest, RenderWritesAntialiasedPixelsAndRecordsWork) {
    constexpr int width = 2;
    constexpr int height = 2;
    Camera camera = makeCamera(width, height);
    Image image(width, height);
    Scene scene;
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 1.0f, Color(1.0f, 1.0f, 1.0f));
    Renderer renderer(camera, image, scene, light, 0.15f);
    renderStats.reset();

    renderer.render();

    const auto stats = renderStats.snapshot();
    EXPECT_EQ(stats.pixelsRendered, static_cast<uint64_t>(width * height));
    EXPECT_EQ(stats.primaryRays, static_cast<uint64_t>(width * height * 16));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            expectColorNear(image.getPixel(x, y), expectedBackgroundPixel(camera, x, y));
        }
    }
}

TEST(RendererTest, SampleCountControlsPrimaryRaysPerPixel) {
    constexpr int width = 2;
    constexpr int height = 2;
    constexpr std::size_t samplesPerPixel = 3;
    Camera camera = makeCamera(width, height);
    Image image(width, height);
    Scene scene;
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 1.0f, Color(1.0f, 1.0f, 1.0f));
    Renderer renderer(camera, image, scene, light, 0.15f);
    renderer.setSamplesPerPixel(samplesPerPixel);
    renderStats.reset();

    renderer.render();

    EXPECT_EQ(
        renderStats.snapshot().primaryRays,
        static_cast<uint64_t>(width * height * samplesPerPixel));
}

TEST(RendererTest, RejectsZeroSamplesPerPixel) {
    Camera camera = makeCamera(1, 1);
    Image image(1, 1);
    Scene scene;
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 1.0f, Color(1.0f, 1.0f, 1.0f));
    Renderer renderer(camera, image, scene, light, 0.15f);

    EXPECT_THROW(renderer.setSamplesPerPixel(0), std::invalid_argument);
}

TEST(RenderStatsTest, MergesThreadLocalTriangleTestCounts) {
    constexpr uint64_t testsPerThread = 10000;
    constexpr size_t threadCount = 4;
    std::vector<uint64_t> triangleTestCounts(threadCount, 0);
    std::vector<std::thread> threads;

    renderStats.reset();
    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back([&, i] {
            RenderStats::TriangleTestCounterScope counterScope(
                renderStats, &triangleTestCounts[i]);
            renderStats.recordTriangleTests(testsPerThread);
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    uint64_t triangleTestCount = 0;
    for (const uint64_t count : triangleTestCounts) {
        triangleTestCount += count;
    }
    renderStats.mergeTriangleTests(triangleTestCount);

    EXPECT_EQ(renderStats.snapshot().triangleTests, testsPerThread * threadCount);
}
