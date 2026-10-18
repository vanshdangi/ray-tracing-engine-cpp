#include <gtest/gtest.h>
#include <world/objects/triangle.hpp>

namespace {

Triangle makeTriangle() {
    return Triangle(
        Point3(0.0f, 0.0f, 0.0f),
        Point3(1.0f, 0.0f, 0.0f),
        Point3(0.0f, 1.0f, 0.0f),
        Material()
    );
}

}

TEST(TriangleTest, RayHitsInsideTriangle) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.25f, 0.25f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, 1.0f);
}

TEST(TriangleTest, RayMissesOutsideTriangle) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.75f, 0.75f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, -1.0f);
}

TEST(TriangleTest, ParallelRayDoesNotIntersect) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.25f, 0.25f, 1.0f), Vec3(1.0f, 0.0f, 0.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, -1.0f);
}

TEST(TriangleTest, RayPointingAwayDoesNotIntersect) {
    Triangle triangle = makeTriangle();
    Ray ray{Point3(0.25f, 0.25f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(triangle.intersects(ray).t, -1.0f);
}

TEST(TriangleTest, NormalIsConstantAcrossTriangle) {
    Triangle triangle = makeTriangle();

    EXPECT_EQ(triangle.getNormal(Point3(0.25f, 0.25f, 0.0f)), Vec3(0.0f, 0.0f, 1.0f));
    EXPECT_EQ(triangle.getNormal(Point3(0.0f, 0.0f, 0.0f)), Vec3(0.0f, 0.0f, 1.0f));
}

class TriangleScaleRegressionTest : public testing::TestWithParam<float> {};

TEST_P(TriangleScaleRegressionTest, RayHitAndMissAreScaleInvariant) {
    const float scale = GetParam();
    Triangle triangle(
        Point3(0.0f, 0.0f, 0.0f),
        Point3(scale, 0.0f, 0.0f),
        Point3(0.0f, scale, 0.0f),
        Material()
    );

    const Ray hitRay{
        Point3(0.25f * scale, 0.25f * scale, -2.0f * scale),
        Vec3(0.0f, 0.0f, 2.0f)
    };
    const Intersection hit = triangle.intersects(hitRay);

    EXPECT_GT(hit.t, 0.0f);
    EXPECT_EQ(hit.object, &triangle);
    EXPECT_EQ(hit.primitive, &triangle);
    EXPECT_NEAR(hit.t / scale, 1.0f, 1e-5f);
    EXPECT_NEAR(hit.u, 0.25f, 1e-5f);
    EXPECT_NEAR(hit.v, 0.25f, 1e-5f);

    const Ray missRay{
        Point3(0.75f * scale, 0.75f * scale, -2.0f * scale),
        Vec3(0.0f, 0.0f, 2.0f)
    };
    EXPECT_FLOAT_EQ(triangle.intersects(missRay).t, -1.0f);
}

INSTANTIATE_TEST_SUITE_P(
    ScalesFromTinyToLarge,
    TriangleScaleRegressionTest,
    testing::Values(1e-3f, 1e-2f, 1.0f, 10.0f, 100.0f)
);
