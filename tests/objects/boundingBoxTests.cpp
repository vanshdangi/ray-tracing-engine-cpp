#include <gtest/gtest.h>
#include <bvh/boundingBox.hpp>

namespace {

BoundingBox makeBox() {
    return BoundingBox(
        Point3(-1.0f, -2.0f, -3.0f),
        Point3(1.0f, 2.0f, 3.0f)
    );
}

}

TEST(BoundingBoxTest, ReportsCenterAndHalfSize) {
    const BoundingBox box = makeBox();

    EXPECT_EQ(box.getCenter(), Point3(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(box.getHalfSize(), Vec3(1.0f, 2.0f, 3.0f));
}

TEST(BoundingBoxTest, RayThroughBoxReturnsNearIntersection) {
    const BoundingBox box = makeBox();
    const Ray ray{Point3(0.0f, 0.0f, -5.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(box.intersects(ray), 2.0f);
}

TEST(BoundingBoxTest, RayStartingInsideReturnsExitIntersection) {
    const BoundingBox box = makeBox();
    const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)};

    EXPECT_FLOAT_EQ(box.intersects(ray), 1.0f);
}

TEST(BoundingBoxTest, ParallelRayOnFaceCanIntersect) {
    const BoundingBox box = makeBox();
    const Ray ray{Point3(1.0f, 0.0f, -5.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(box.intersects(ray), 2.0f);
}

TEST(BoundingBoxTest, ParallelRayOutsideSlabMisses) {
    const BoundingBox box = makeBox();
    const Ray ray{Point3(2.0f, 0.0f, -5.0f), Vec3(0.0f, 0.0f, 1.0f)};

    EXPECT_FLOAT_EQ(box.intersects(ray), -1.0f);
}
