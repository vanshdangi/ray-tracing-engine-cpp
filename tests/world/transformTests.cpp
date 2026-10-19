#include <gtest/gtest.h>
#include <world/transform.hpp>

#include <cmath>

TEST(TransformTest, DefaultTransformIsIdentity) {
    const Transform transform;

    EXPECT_EQ(transform.getMatrix(), Matrix4::identity());
    EXPECT_EQ(transform.getInverseMatrix(), Matrix4::identity());
    EXPECT_EQ(transform.toWorld(Point3(1.0f, 2.0f, 3.0f)),
              Point3(1.0f, 2.0f, 3.0f));
}

TEST(TransformTest, AppliesTranslationRotationAndScale) {
    const Transform transform(
        Point3(3.0f, -2.0f, 5.0f),
        Vec3(0.0f, 0.0f, 90.0f),
        Vec3(2.0f, 3.0f, 4.0f)
    );

    const Point3 worldPoint = transform.toWorld(Point3(1.0f, 0.0f, 0.0f));

    EXPECT_NEAR(worldPoint.x, 3.0f, 1e-5f);
    EXPECT_NEAR(worldPoint.y, 0.0f, 1e-5f);
    EXPECT_NEAR(worldPoint.z, 5.0f, 1e-5f);
}

TEST(TransformTest, LocalRayRoundTripsThroughWorldTransform) {
    const Transform transform(
        Point3(4.0f, -3.0f, 2.0f),
        Vec3(20.0f, -35.0f, 15.0f),
        Vec3(2.0f, 0.5f, 3.0f)
    );
    const Ray localRay{
        Point3(1.0f, 2.0f, -1.0f),
        Vec3(0.0f, 0.0f, 1.0f)
    };
    const Matrix4 worldMatrix = transform.getMatrix();
    const Ray worldRay{
        worldMatrix * localRay.origin,
        worldMatrix * localRay.direction
    };
    const Ray convertedRay = transform.toLocal(worldRay);

    EXPECT_NEAR(convertedRay.origin.x, localRay.origin.x, 1e-5f);
    EXPECT_NEAR(convertedRay.origin.y, localRay.origin.y, 1e-5f);
    EXPECT_NEAR(convertedRay.origin.z, localRay.origin.z, 1e-5f);
    EXPECT_NEAR(convertedRay.direction.x, localRay.direction.x, 1e-5f);
    EXPECT_NEAR(convertedRay.direction.y, localRay.direction.y, 1e-5f);
    EXPECT_NEAR(convertedRay.direction.z, localRay.direction.z, 1e-5f);
}

TEST(TransformTest, InverseMatrixReversesForwardTransform) {
    const Transform transform(
        Point3(-2.0f, 4.0f, 1.0f),
        Vec3(25.0f, 40.0f, -10.0f),
        Vec3(0.5f, 2.0f, 3.0f)
    );
    const Point3 point(3.0f, -1.0f, 2.0f);
    const Point3 restored =
        transform.getInverseMatrix() * transform.getMatrix() * point;

    EXPECT_NEAR(restored.x, point.x, 1e-5f);
    EXPECT_NEAR(restored.y, point.y, 1e-5f);
    EXPECT_NEAR(restored.z, point.z, 1e-5f);
}

TEST(TransformTest, TransformsNormalsUsingInverseScale) {
    const Transform transform(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 90.0f),
        Vec3(2.0f, 1.0f, 1.0f)
    );

    const Vec3 normal = transform.transformNormal(Vec3(1.0f, 1.0f, 0.0f));
    const float inverseSqrtFive = 1.0f / std::sqrt(5.0f);

    EXPECT_NEAR(normal.x, -2.0f * inverseSqrtFive, 1e-5f);
    EXPECT_NEAR(normal.y, inverseSqrtFive, 1e-5f);
    EXPECT_NEAR(normal.z, 0.0f, 1e-5f);
}
