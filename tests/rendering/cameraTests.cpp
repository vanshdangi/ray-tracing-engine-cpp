#include <gtest/gtest.h>
#include <rendering/camera.hpp>

namespace {

Camera makeCamera() {
	return Camera(
		Point3(0.0f, 0.0f, 0.0f),
		Vec3(0.0f, 0.0f, -1.0f),
		90.0f,
		1.0f,
		3.0f,
		3.0f
	);
}

} // namespace

TEST(CameraTest, CenterPixelPointsForward) {
	Camera camera = makeCamera();

	Ray ray = camera.generateRay(1, 1, 0.5f, 0.5f);

	EXPECT_EQ(ray.origin, Point3(0.0f, 0.0f, 0.0f));
	EXPECT_EQ(ray.direction, Vec3(0.0f, 0.0f, -1.0f));
}

TEST(CameraTest, RightSidePointsRight) {
	Camera camera = makeCamera();

	Ray ray = camera.generateRay(2, 1, 0.5f, 0.5f);

	EXPECT_GT(ray.direction.x, 0.0f);
	EXPECT_FLOAT_EQ(ray.direction.y, 0.0f);
	EXPECT_LT(ray.direction.z, 0.0f);
}

TEST(CameraTest, LeftSidePointsNegativeRight) {
	Camera camera = makeCamera();

	Ray ray = camera.generateRay(0, 1, 0.5f, 0.5f);

	EXPECT_LT(ray.direction.x, 0.0f);
	EXPECT_FLOAT_EQ(ray.direction.y, 0.0f);
	EXPECT_LT(ray.direction.z, 0.0f);
}

TEST(CameraTest, TopPointsUp) {
	Camera camera = makeCamera();

	Ray ray = camera.generateRay(1, 0, 0.5f, 0.5f);

	EXPECT_FLOAT_EQ(ray.direction.x, 0.0f);
	EXPECT_GT(ray.direction.y, 0.0f);
	EXPECT_LT(ray.direction.z, 0.0f);
}

TEST(CameraTest, BottomPointsDown) {
	Camera camera = makeCamera();

	Ray ray = camera.generateRay(1, 2, 0.5f, 0.5f);

	EXPECT_FLOAT_EQ(ray.direction.x, 0.0f);
	EXPECT_LT(ray.direction.y, 0.0f);
	EXPECT_LT(ray.direction.z, 0.0f);
}
