#include <gtest/gtest.h>
#include <world/objects/aabb.hpp>

namespace {

Transform identityTransform() {
    return Transform(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
}

} // namespace

TEST(AABBTest, RayThroughCenter) {
	AABB box(identityTransform(), 1.0f, Material());
	Ray ray{Point3(0.0f, 0.0f, -3.0f), Vec3(0.0f, 0.0f, 1.0f)};

	EXPECT_FLOAT_EQ(box.intersects(ray).t, 2.0f);
}

TEST(AABBTest, RayMissesBox) {
	AABB box(identityTransform(), 1.0f, Material());
	Ray ray{Point3(2.0f, 0.0f, -3.0f), Vec3(0.0f, 0.0f, 1.0f)};

	EXPECT_FLOAT_EQ(box.intersects(ray).t, -1.0f);
}

TEST(AABBTest, RayStartsInsideBox) {
	AABB box(identityTransform(), 1.0f, Material());
	Ray ray{Point3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)};

	EXPECT_FLOAT_EQ(box.intersects(ray).t, 1.0f);
}

TEST(AABBTest, RayParallelToFace) {
	AABB box(identityTransform(), 1.0f, Material());
	Ray ray{Point3(1.0f, 0.0f, -3.0f), Vec3(0.0f, 0.0f, 1.0f)};

	EXPECT_FLOAT_EQ(box.intersects(ray).t, 2.0f);
}

TEST(AABBTest, RayHitsExactlyAtBoundary) {
	AABB box(identityTransform(), 1.0f, Material());
	Ray ray{Point3(2.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f)};

	EXPECT_FLOAT_EQ(box.intersects(ray).t, 1.0f);
}
