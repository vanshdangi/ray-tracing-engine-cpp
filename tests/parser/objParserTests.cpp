#include <gtest/gtest.h>
#include <parser/objParser.hpp>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace {

class TemporaryOBJ {
public:
    explicit TemporaryOBJ(const std::string& contents) {
        const auto* testInfo = testing::UnitTest::GetInstance()->current_test_info();
        path = std::filesystem::temp_directory_path() /
            (std::string("ray_tracing_") + testInfo->test_suite_name() + "_" +
             testInfo->name() + ".obj");
        std::ofstream file(path);
        if (!file) {
            throw std::runtime_error("Could not create temporary OBJ test file");
        }
        file << contents;
        if (!file) {
            throw std::runtime_error("Could not write temporary OBJ test file");
        }
    }

    ~TemporaryOBJ() {
        std::error_code error;
        std::filesystem::remove(path, error);
    }

    const std::filesystem::path& getPath() const {
        return path;
    }

private:
    std::filesystem::path path;
};

} // namespace

TEST(OBJParserTest, ReadsVerticesAndTriangulatesPolygon) {
    TemporaryOBJ file(
        "# square\n"
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 1 1 0\n"
        "v 0 1 0\n"
        "f 1 2 3 4 # fan triangulation\n"
    );

    const OBJData data = OBJParser().parser(file.getPath().string());

    ASSERT_EQ(data.vertices.size(), 4u);
    ASSERT_EQ(data.triangles.size(), 2u);
    ASSERT_EQ(data.triangleIndices.size(), 2u);
    EXPECT_EQ(data.triangleIndices[0].vertices[0].vertex, 0);
    EXPECT_EQ(data.triangleIndices[0].vertices[1].vertex, 1);
    EXPECT_EQ(data.triangleIndices[0].vertices[2].vertex, 2);
    EXPECT_EQ(data.triangleIndices[1].vertices[0].vertex, 0);
    EXPECT_EQ(data.triangleIndices[1].vertices[1].vertex, 2);
    EXPECT_EQ(data.triangleIndices[1].vertices[2].vertex, 3);
}

TEST(OBJParserTest, ReadsPerCornerTextureCoordinatesAndNormals) {
    TemporaryOBJ file(
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "vt 0 0\n"
        "vt 1 0\n"
        "vt 0 1\n"
        "vn 0 0 1\n"
        "f 1/1/1 2/2/1 3/3/1\n"
    );

    const OBJData data = OBJParser().parser(file.getPath().string());

    ASSERT_EQ(data.texCoords.size(), 3u);
    ASSERT_EQ(data.normals.size(), 1u);
    ASSERT_EQ(data.triangleIndices.size(), 1u);
    const auto& corners = data.triangleIndices[0].vertices;
    EXPECT_EQ(corners[0].texCoord, 0);
    EXPECT_EQ(corners[1].texCoord, 1);
    EXPECT_EQ(corners[2].texCoord, 2);
    EXPECT_EQ(corners[0].normal, 0);
    EXPECT_EQ(corners[1].normal, 0);
    EXPECT_EQ(corners[2].normal, 0);
}

TEST(OBJParserTest, ResolvesNegativeIndicesAndAllowsMissingTextureCoordinate) {
    TemporaryOBJ file(
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "vn 0 0 1\n"
        "f -3//-1 -2//-1 -1//-1\n"
    );

    const OBJData data = OBJParser().parser(file.getPath().string());

    ASSERT_EQ(data.triangleIndices.size(), 1u);
    for (std::size_t index = 0; index < 3; ++index) {
        EXPECT_EQ(data.triangleIndices[0].vertices[index].vertex,
                  static_cast<int>(index));
        EXPECT_EQ(data.triangleIndices[0].vertices[index].texCoord, -1);
        EXPECT_EQ(data.triangleIndices[0].vertices[index].normal, 0);
    }
}

TEST(OBJParserTest, RejectsMalformedFaceIndex) {
    TemporaryOBJ file(
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2/ 3\n"
    );

    const OBJData data = OBJParser().parser(file.getPath().string());

    EXPECT_TRUE(data.vertices.empty());
    EXPECT_TRUE(data.triangles.empty());
}

TEST(OBJParserTest, RejectsZeroAndOutOfRangeIndices) {
    EXPECT_THROW(parseOBJIndex(0, 3), std::runtime_error);
    EXPECT_THROW(parseOBJIndex(4, 3), std::runtime_error);
    EXPECT_THROW(parseOBJIndex(-4, 3), std::runtime_error);
    EXPECT_EQ(parseOBJIndex(-1, 3), 2);
}
