#include <gtest/gtest.h>
#include <world/objects/sphere.hpp>

namespace {

Transform identityTransform() {
    return Transform(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
}

} // namespace

TEST(SphereTest, RayDirectlyAtSphere) {
    Sphere sphere(identityTransform(), 1.0f, Material());
    Ray ray{Point3(0.0f, 0.0f, -3.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(sphere.intersects(ray).t, 2.0f);
}

TEST(SphereTest, RayMissesSphere) {
    Sphere sphere(identityTransform(), 1.0f, Material());
    Ray ray{Point3(0.0f, 0.0f, -3.0f), Vec3(0.0f, 1.0f, 0.0f)};

    EXPECT_FLOAT_EQ(sphere.intersects(ray).t, -1.0f);
}

TEST(SphereTest, RayStartsInsideSphere) {
    Sphere sphere(identityTransform(), 1.0f, Material());
    Ray ray{Point3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)};

    EXPECT_FLOAT_EQ(sphere.intersects(ray).t, 1.0f);
}

TEST(SphereTest, RayStartsBehindSphere) {
    Sphere sphere(identityTransform(), 1.0f, Material());
    Ray ray{Point3(0.0f, 0.0f, 3.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(sphere.intersects(ray).t, -1.0f);
}

TEST(SphereTest, TangentRayHasOneIntersection) {
    Sphere sphere(identityTransform(), 1.0f, Material());
    Ray ray{Point3(1.0f, 0.0f, -3.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(sphere.intersects(ray).t, 3.0f);
}