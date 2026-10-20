#include <gtest/gtest.h>
#include <render_stats.hpp>
#include <rendering/renderer.hpp>
#include <world/hitRecord.hpp>
#include <world/objects/mesh.hpp>
#include <world/objects/sphere.hpp>
#include <world/objects/triangle.hpp>

#include <array>
#include <cmath>
#include <memory>
#include <random>
#include <utility>
#include <vector>

namespace {

class RenderingCorrectnessTest : public ::testing::Test {
protected:
    void SetUp() override {
        renderStats.reset();
        Mesh::setBVHEnabled(true);
    }

    void TearDown() override {
        Mesh::setBVHEnabled(true);
        renderStats.reset();
    }
};

Camera makeCamera() {
    return Camera(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 1.0f),
        90.0f,
        1.0f,
        1.0f,
        1.0f
    );
}

void expectColorNear(const Color& actual, const Color& expected, float tolerance = 1e-5f) {
    EXPECT_NEAR(actual.r, expected.r, tolerance);
    EXPECT_NEAR(actual.g, expected.g, tolerance);
    EXPECT_NEAR(actual.b, expected.b, tolerance);
}

void expectVecNear(const Vec3& actual, const Vec3& expected, float tolerance = 1e-5f) {
    EXPECT_NEAR(actual.x, expected.x, tolerance);
    EXPECT_NEAR(actual.y, expected.y, tolerance);
    EXPECT_NEAR(actual.z, expected.z, tolerance);
}

Transform translatedToZ(float z) {
    return Transform(
        Point3(0.0f, 0.0f, z),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
}

std::unique_ptr<Triangle> makeSurface(float z, const Material& material, bool reversed = false) {
    if (reversed) {
        return std::make_unique<Triangle>(
            Point3(-2.0f, -2.0f, z),
            Point3(0.0f, 2.0f, z),
            Point3(2.0f, -2.0f, z),
            material
        );
    }
    return std::make_unique<Triangle>(
        Point3(-2.0f, -2.0f, z),
        Point3(2.0f, -2.0f, z),
        Point3(0.0f, 2.0f, z),
        material
    );
}

HitRecord surfaceHit(const Triangle& surface, const Ray& ray) {
    const Intersection intersection = surface.intersects(ray);
    return HitRecord(ray.at(intersection.t), intersection);
}

Color lightSurface(
    bool reversed,
    PointLight& light,
    float ambientIntensity,
    bool addOccluder = false,
    float occluderZ = 0.0f
) {
    Material material;
    material.albedo = Color(0.5f, 0.25f, 0.75f);

    Scene scene;
    auto surface = makeSurface(3.0f, material, reversed);
    const Triangle* surfacePointer = surface.get();
    scene.addObject(std::move(surface));

    if (addOccluder) {
        Material blockerMaterial;
        scene.addObject(makeSurface(occluderZ, blockerMaterial));
    }

    Image image(1, 1);
    Camera camera = makeCamera();
    const Renderer renderer(camera, image, scene, light, ambientIntensity);
    const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)};
    return renderer.calculateLighting(surfaceHit(*surfacePointer, ray), ray);
}

OBJData makeGridMeshData() {
    OBJData data;
    for (int i = 0; i < 10; ++i) {
        const float x = static_cast<float>((i % 5) * 2 - 4);
        const float y = static_cast<float>((i / 5) * 2 - 1);
        data.triangles.emplace_back(
            Point3(x, y, 5.0f),
            Point3(x + 1.0f, y, 5.0f),
            Point3(x, y + 1.0f, 5.0f),
            Material()
        );
    }

    // These triangles overlap in projection; the z=3 primitive is the nearest hit.
    data.triangles.emplace_back(
        Point3(-0.5f, -0.5f, 3.0f),
        Point3(0.5f, -0.5f, 3.0f),
        Point3(0.0f, 0.5f, 3.0f),
        Material()
    );
    data.triangles.emplace_back(
        Point3(-0.5f, -0.5f, 4.0f),
        Point3(0.5f, -0.5f, 4.0f),
        Point3(0.0f, 0.5f, 4.0f),
        Material()
    );
    return data;
}

struct RayHitResult {
    bool hit;
    float t;
    const Triangle* primitive;
    float u;
    float v;
};

RayHitResult intersectWithMode(Mesh& mesh, const Ray& ray, bool bvhEnabled) {
    Mesh::setBVHEnabled(bvhEnabled);
    const Intersection intersection = mesh.intersects(ray);
    return {
        intersection.t >= 0.0f,
        intersection.t,
        intersection.primitive,
        intersection.u,
        intersection.v
    };
}

} // namespace

TEST_F(RenderingCorrectnessTest, BVHMatchesBruteForceForSeededRaysEdgesMissesAndOverlaps) {
    Mesh mesh(Transform(), makeGridMeshData(), Material());
    std::mt19937 generator(0x5eedu);
    std::uniform_real_distribution<float> barycentric(0.05f, 0.45f);
    std::vector<Ray> rays;

    for (int i = 0; i < 10; ++i) {
        const float x = static_cast<float>((i % 5) * 2 - 4);
        const float y = static_cast<float>((i / 5) * 2 - 1);
        float u = barycentric(generator);
        float v = barycentric(generator);
        if (u + v >= 0.9f) {
            v = 0.9f - u;
        }
        const Point3 target(x + u, y + v, 5.0f);
        const Point3 origin(target.x - 0.3f, target.y + 0.2f, 0.0f);
        rays.push_back({origin, target - origin});
    }

    rays.push_back({Point3(-4.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, 5.0f)});
    rays.push_back({Point3(-3.5f, -0.5f, 0.0f), Vec3(0.0f, 0.0f, 5.0f)});
    rays.push_back({Point3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 5.0f)});
    rays.push_back({Point3(20.0f, 20.0f, 0.0f), Vec3(0.0f, 0.0f, 5.0f)});
    rays.push_back({Point3(-20.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 5.0f)});

    for (const Ray& ray : rays) {
        const RayHitResult bruteForce = intersectWithMode(mesh, ray, false);
        const RayHitResult bvh = intersectWithMode(mesh, ray, true);

        ASSERT_EQ(bvh.hit, bruteForce.hit);
        if (!bruteForce.hit) {
            EXPECT_EQ(bvh.primitive, nullptr);
            continue;
        }

        EXPECT_NEAR(bvh.t, bruteForce.t, 1e-5f);
        EXPECT_EQ(bvh.primitive, bruteForce.primitive);
        EXPECT_NEAR(bvh.u, bruteForce.u, 1e-5f);
        EXPECT_NEAR(bvh.v, bruteForce.v, 1e-5f);
    }

    const Ray overlappingRay{
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 1.0f)
    };
    const RayHitResult nearest = intersectWithMode(mesh, overlappingRay, true);
    ASSERT_TRUE(nearest.hit);
    EXPECT_NEAR(nearest.t, 3.0f, 1e-5f);
    ASSERT_NE(nearest.primitive, nullptr);
    EXPECT_FLOAT_EQ(nearest.primitive->getA().z, 3.0f);
}

TEST_F(RenderingCorrectnessTest, ReversedWindingFlipsGeometricNormalButPreservesRendererOrientation) {
    Material material;
    material.albedo = Color(0.5f, 0.25f, 0.75f);
    auto forward = makeSurface(3.0f, material);
    auto reversed = makeSurface(3.0f, material, true);
    const Ray ray{Point3(), Vec3(0.0f, 0.0f, 1.0f)};
    const HitRecord forwardHit = surfaceHit(*forward, ray);
    const HitRecord reversedHit = surfaceHit(*reversed, ray);

    EXPECT_NEAR(forwardHit.t, reversedHit.t, 1e-5f);
    EXPECT_NEAR(forwardHit.normal.z, 1.0f, 1e-5f);
    EXPECT_NEAR(reversedHit.normal.z, -1.0f, 1e-5f);

    PointLight light(Point3(0.0f, 0.0f, 0.0f), 1.0f, Color(1.0f, 1.0f, 1.0f));
    expectColorNear(
        lightSurface(false, light, 0.0f),
        Color(0.5f, 0.25f, 0.75f)
    );
    expectColorNear(
        lightSurface(true, light, 0.0f),
        Color(0.5f, 0.25f, 0.75f)
    );
}

TEST_F(RenderingCorrectnessTest, InterpolatesAndNormalizesVertexShadingNormals) {
    OBJData data;
    data.triangles.emplace_back(
        Point3(0.0f, 0.0f, 0.0f),
        Point3(2.0f, 0.0f, 0.0f),
        Point3(0.0f, 2.0f, 0.0f),
        Material()
    );
    data.normals = {
        Vec3(0.0f, 0.0f, 2.0f),
        Vec3(0.0f, 2.0f, 2.0f),
        Vec3(2.0f, 0.0f, 2.0f)
    };
    OBJTriangleIndices indices;
    indices.vertices[0] = {0, -1, 0};
    indices.vertices[1] = {1, -1, 1};
    indices.vertices[2] = {2, -1, 2};
    data.triangleIndices.push_back(indices);
    Mesh mesh(translatedToZ(3.0f), std::move(data), Material());

    const Ray ray{Point3(0.5f, 0.5f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)};
    const Intersection intersection = mesh.intersects(ray);
    ASSERT_GE(intersection.t, 0.0f);
    EXPECT_NEAR(intersection.u, 0.25f, 1e-5f);
    EXPECT_NEAR(intersection.v, 0.25f, 1e-5f);

    const Vec3 expected = Vec3(0.5f, 0.5f, 2.0f).normalized();
    const Vec3 shadingNormal = mesh.getShadingNormal(intersection, ray.at(intersection.t));
    expectVecNear(shadingNormal, expected);
    EXPECT_NEAR(shadingNormal.magnitude(), 1.0f, 1e-5f);
}

TEST_F(RenderingCorrectnessTest, MissingVertexNormalsFallBackToGeometricNormal) {
    OBJData data;
    data.triangles.emplace_back(
        Point3(0.0f, 0.0f, 0.0f),
        Point3(2.0f, 0.0f, 0.0f),
        Point3(0.0f, 2.0f, 0.0f),
        Material()
    );
    Scene scene;
    Material meshMaterial;
    meshMaterial.albedo = Color(0.4f, 0.8f, 0.2f);
    scene.addObject(std::make_unique<Mesh>(translatedToZ(3.0f), std::move(data), meshMaterial));
    const Mesh* mesh = static_cast<const Mesh*>(scene.getObjects()[0].get());

    const Ray ray{Point3(0.5f, 0.5f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)};
    const Intersection intersection = mesh->intersects(ray);
    ASSERT_GE(intersection.t, 0.0f);
    expectVecNear(mesh->getShadingNormal(intersection, ray.at(intersection.t)), Vec3(0.0f, 0.0f, 1.0f));

    PointLight light(Point3(0.0f, 0.0f, 0.0f), 0.0f, Color(1.0f, 1.0f, 1.0f));
    Image image(1, 1);
    Camera camera = makeCamera();
    const Renderer renderer(camera, image, scene, light, 0.25f);
    const Color traced = renderer.traceRay(ray, 0);
    expectColorNear(traced, Color(0.1f, 0.2f, 0.05f));
}

TEST_F(RenderingCorrectnessTest, InvalidZeroLengthVertexNormalsFallBackToGeometricNormal) {
    OBJData data;
    data.triangles.emplace_back(
        Point3(0.0f, 0.0f, 0.0f),
        Point3(2.0f, 0.0f, 0.0f),
        Point3(0.0f, 2.0f, 0.0f),
        Material()
    );
    data.normals = {Vec3(), Vec3(), Vec3()};
    OBJTriangleIndices indices;
    indices.vertices[0] = {0, -1, 0};
    indices.vertices[1] = {1, -1, 1};
    indices.vertices[2] = {2, -1, 2};
    data.triangleIndices.push_back(indices);
    Mesh mesh(translatedToZ(3.0f), std::move(data), Material());

    const Ray ray{Point3(0.5f, 0.5f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)};
    const Intersection intersection = mesh.intersects(ray);
    ASSERT_GE(intersection.t, 0.0f);
    expectVecNear(mesh.getShadingNormal(intersection, ray.at(intersection.t)), Vec3(0.0f, 0.0f, 1.0f));
}

TEST_F(RenderingCorrectnessTest, OctahedralTriangleMeshMatchesAnalyticSphereAtPole) {
    OBJData data;
    const Point3 north(0.0f, 0.0f, 1.0f);
    const Point3 south(0.0f, 0.0f, -1.0f);
    const std::array<Point3, 4> ring{
        Point3(1.0f, 0.0f, 0.0f),
        Point3(0.0f, 1.0f, 0.0f),
        Point3(-1.0f, 0.0f, 0.0f),
        Point3(0.0f, -1.0f, 0.0f)
    };
    for (std::size_t i = 0; i < ring.size(); ++i) {
        const Point3& next = ring[(i + 1) % ring.size()];
        data.triangles.emplace_back(north, ring[i], next, Material());
        data.triangles.emplace_back(south, next, ring[i], Material());
    }

    Mesh::setBVHEnabled(false);
    Mesh triangulatedSphere(Transform(), std::move(data), Material());
    Sphere analyticSphere(Transform(), 1.0f, Material());
    const Ray ray{Point3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f)};

    const Intersection meshHit = triangulatedSphere.intersects(ray);
    const Intersection sphereHit = analyticSphere.intersects(ray);
    ASSERT_GE(meshHit.t, 0.0f);
    ASSERT_GE(sphereHit.t, 0.0f);
    EXPECT_NEAR(meshHit.t, sphereHit.t, 1e-5f);
}

TEST_F(RenderingCorrectnessTest, AmbientAndDiffuseLightingRespectSurfaceOrientationAndScale) {
    PointLight disabledLight(
        Point3(0.0f, 0.0f, 0.0f),
        0.0f,
        Color(1.0f, 1.0f, 1.0f)
    );
    expectColorNear(
        lightSurface(false, disabledLight, 0.2f),
        Color(0.1f, 0.05f, 0.15f)
    );
    expectColorNear(
        lightSurface(false, disabledLight, 0.6f),
        Color(0.3f, 0.15f, 0.45f)
    );
    expectColorNear(lightSurface(false, disabledLight, 0.0f), Color());

    PointLight frontLight(
        Point3(0.0f, 0.0f, 0.0f),
        1.0f,
        Color(1.0f, 1.0f, 1.0f)
    );
    expectColorNear(
        lightSurface(false, frontLight, 0.0f),
        Color(0.5f, 0.25f, 0.75f)
    );

    PointLight behindLight(
        Point3(0.0f, 0.0f, 6.0f),
        1.0f,
        Color(1.0f, 1.0f, 1.0f)
    );
    expectColorNear(lightSurface(false, behindLight, 0.0f), Color());
}

TEST_F(RenderingCorrectnessTest, ShadowsOnlyConsiderOccludersBetweenSurfaceAndLight) {
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 1.0f, Color(1.0f, 1.0f, 1.0f));
    const Color direct(0.5f, 0.25f, 0.75f);

    expectColorNear(lightSurface(false, light, 0.0f), direct);
    expectColorNear(lightSurface(false, light, 0.0f, true, 1.5f), Color());
    expectColorNear(lightSurface(false, light, 0.0f, true, -1.0f), direct);
}

TEST_F(RenderingCorrectnessTest, ReflectionsReturnKnownObjectColorAndRespectReflectivityAndDepth) {
    Material mirror;
    mirror.albedo = Color();
    mirror.reflectivity = 1.0f;
    Material red;
    red.albedo = Color(1.0f, 0.0f, 0.0f);

    Scene scene;
    scene.addObject(makeSurface(3.0f, mirror));
    scene.addObject(std::make_unique<Sphere>(translatedToZ(-2.0f), 0.5f, red));
    Image image(1, 1);
    Camera camera = makeCamera();
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 0.0f, Color(1.0f, 1.0f, 1.0f));
    const Renderer renderer(camera, image, scene, light, 0.5f);
    const Ray ray{Point3(), Vec3(0.0f, 0.0f, 1.0f)};
    expectColorNear(renderer.traceRay(ray, 0), Color(0.5f, 0.0f, 0.0f));
    expectColorNear(renderer.traceRay(ray, 3), Color());

    Material matte = mirror;
    matte.albedo = Color(0.0f, 0.4f, 0.2f);
    matte.reflectivity = 0.0f;
    Scene matteScene;
    matteScene.addObject(makeSurface(3.0f, matte));
    matteScene.addObject(std::make_unique<Sphere>(translatedToZ(-2.0f), 0.5f, red));
    const Renderer matteRenderer(camera, image, matteScene, light, 0.5f);
    expectColorNear(matteRenderer.traceRay(ray, 0), Color(0.0f, 0.2f, 0.1f));
}

TEST_F(RenderingCorrectnessTest, ReflectionOffsetPreventsAnImmediateSphereSelfHit) {
    Material reflective;
    reflective.albedo = Color();
    reflective.reflectivity = 1.0f;
    Scene scene;
    scene.addObject(std::make_unique<Sphere>(translatedToZ(3.0f), 1.0f, reflective));
    Image image(1, 1);
    Camera camera = makeCamera();
    PointLight light(Point3(0.0f, 0.0f, 0.0f), 0.0f, Color(1.0f, 1.0f, 1.0f));
    const Renderer renderer(camera, image, scene, light, 0.0f);
    renderStats.reset();

    renderer.traceRay(Ray{Point3(), Vec3(0.0f, 0.0f, 1.0f)}, 0);

    EXPECT_EQ(renderStats.snapshot().raysTraced, 2);
}
