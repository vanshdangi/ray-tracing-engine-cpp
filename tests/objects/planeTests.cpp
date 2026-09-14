#include <gtest/gtest.h>
#include <world/objects/plane.hpp>

namespace {

Transform identityTransform() {
    return Transform(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
}

} // namespace

TEST(PlaneTest, RayTowardsPlane) {
	Plane plane(identityTransform(), Vec3(0.0f, 1.0f, 0.0f), Material());
	Ray ray{Point3(0.0f, 1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)};

	EXPECT_FLOAT_EQ(plane.intersects(ray).t, 1.0f);
}

TEST(PlaneTest, RayParallelToPlane) {
	Plane plane(identityTransform(), Vec3(0.0f, 1.0f, 0.0f), Material());
	Ray ray{Point3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)};

	EXPECT_FLOAT_EQ(plane.intersects(ray).t, -1.0f);
}

TEST(PlaneTest, RayPointingAwayFromPlane) {
	Plane plane(identityTransform(), Vec3(0.0f, 1.0f, 0.0f), Material());
	Ray ray{Point3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)};

	EXPECT_FLOAT_EQ(plane.intersects(ray).t, -1.0f);
}

TEST(PlaneTest, RayLyingInPlane) {
	Plane plane(identityTransform(), Vec3(0.0f, 1.0f, 0.0f), Material());
	Ray ray{Point3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)};

	EXPECT_FLOAT_EQ(plane.intersects(ray).t, -1.0f);
}
