#include <gtest/gtest.h>
#include <world/objects/mesh.hpp>

#include <utility>

namespace {

OBJData makeMeshData() {
    OBJData data;
    const Material material;

    for (int index = 0; index < 5; ++index) {
        const float x = static_cast<float>(index * 2);
        data.triangles.emplace_back(
            Point3(x, 0.0f, 0.0f),
            Point3(x + 1.0f, 0.0f, 0.0f),
            Point3(x, 1.0f, 0.0f),
            material
        );
    }

    return data;
}

Transform translatedMeshTransform() {
    return Transform(
        Point3(0.0f, 0.0f, 2.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
}

} // namespace

TEST(MeshTest, BVHFindsIntersectionsAcrossMultipleLeaves) {
    Mesh mesh(translatedMeshTransform(), makeMeshData(), Material());
    const Ray ray{Point3(8.25f, 0.25f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)};

    const Intersection hit = mesh.intersects(ray);

    EXPECT_FLOAT_EQ(hit.t, 2.0f);
    EXPECT_EQ(hit.object, &mesh);
    ASSERT_NE(hit.primitive, nullptr);
    EXPECT_EQ(hit.primitive->getA(), Point3(8.0f, 0.0f, 0.0f));
}

TEST(MeshTest, BVHRejectsRayOutsideMeshBounds) {
    Mesh mesh(translatedMeshTransform(), makeMeshData(), Material());
    const Ray ray{Point3(20.0f, 0.25f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)};

    const Intersection hit = mesh.intersects(ray);

    EXPECT_FLOAT_EQ(hit.t, -1.0f);
    EXPECT_EQ(hit.object, nullptr);
    EXPECT_EQ(hit.primitive, nullptr);
}

TEST(MeshTest, ReturnsNearestHitFromOverlappingTriangles) {
    OBJData data;
    data.triangles.emplace_back(
        Point3(0.0f, 0.0f, 2.0f),
        Point3(1.0f, 0.0f, 2.0f),
        Point3(0.0f, 1.0f, 2.0f),
        Material()
    );
    data.triangles.emplace_back(
        Point3(0.0f, 0.0f, 0.0f),
        Point3(1.0f, 0.0f, 0.0f),
        Point3(0.0f, 1.0f, 0.0f),
        Material()
    );
    Mesh mesh(Transform(), std::move(data), Material());
    const Ray ray{Point3(0.25f, 0.25f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)};

    const Intersection hit = mesh.intersects(ray);

    EXPECT_FLOAT_EQ(hit.t, 1.0f);
    ASSERT_NE(hit.primitive, nullptr);
    EXPECT_EQ(hit.primitive->getA(), Point3(0.0f, 0.0f, 0.0f));
}

TEST(MeshTest, FindsNearestIntersectionWithoutBVH) {
    Mesh mesh(translatedMeshTransform(), makeMeshData(), Material());
    const Ray ray{Point3(8.25f, 0.25f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)};
    Mesh::setBVHEnabled(false);

    const Intersection hit = mesh.intersects(ray);

    Mesh::setBVHEnabled(true);
    EXPECT_FLOAT_EQ(hit.t, 2.0f);
    EXPECT_EQ(hit.object, &mesh);
    ASSERT_NE(hit.primitive, nullptr);
    EXPECT_EQ(hit.primitive->getA(), Point3(8.0f, 0.0f, 0.0f));
}
