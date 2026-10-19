
#include <parser/objParser.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <cstdint>
#include <string>
#include <vector>

#include <core/material.hpp>

namespace {

// Parse an integer strictly: reject empty strings and trailing junk.
int parseInteger(const std::string& text) {
    if (text.empty()) {
        throw std::runtime_error("Missing OBJ index");
    }

    std::size_t charsRead = 0;
    int value = std::stoi(text, &charsRead);

    if (charsRead != text.size()) {
        throw std::runtime_error("Malformed OBJ index: " + text);
    }

    return value;
}

// Parse one face token, such as:
// 1, 1/2, 1//3, or 1/2/3.
OBJVertexIndex parseFaceVertex(
    const std::string& token,
    const OBJData& data
) {
    OBJVertexIndex result;

    const std::size_t firstSlash = token.find('/');
    const std::size_t secondSlash =
        firstSlash == std::string::npos
            ? std::string::npos
            : token.find('/', firstSlash + 1);

    // More than two separators is invalid.
    if (secondSlash != std::string::npos &&
        token.find('/', secondSlash + 1) != std::string::npos) {
        throw std::runtime_error(
            "Too many '/' separators in face token: " + token
        );
    }

    const std::string vertexText =
        token.substr(0, firstSlash);

    result.vertex = parseOBJIndex(
        parseInteger(vertexText),
        data.vertices.size()
    );

    if (firstSlash == std::string::npos) {
        return result;
    }

    const std::size_t texCoordStart = firstSlash + 1;

    const std::string texCoordText =
        token.substr(
            texCoordStart,
            secondSlash == std::string::npos
                ? std::string::npos
                : secondSlash - texCoordStart
        );

    if (secondSlash == std::string::npos) {
        // v/vt format: the texture coordinate is required.
        result.texCoord = parseOBJIndex(
            parseInteger(texCoordText),
            data.texCoords.size()
        );

        return result;
    }

    // v//vn or v/vt/vn
    // An empty texture coordinate is allowed here.
    if (!texCoordText.empty()) {
        result.texCoord = parseOBJIndex(
            parseInteger(texCoordText),
            data.texCoords.size()
        );
    }

    const std::string normalText =
        token.substr(secondSlash + 1);

    result.normal = parseOBJIndex(
        parseInteger(normalText),
        data.normals.size()
    );

    return result;
}

} // namespace

OBJData OBJParser::parser(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Failed to open OBJ: "
                  << filename << '\n';
        return {};
    }

    // Use local data so repeated calls don't append to old meshes.
    OBJData data;

    Material mat;
    // Temporary material until MTL loading is implemented.
    mat.albedo = Color::fromSrgb(0.784f, 0.196f, 0.196f);

    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        std::istringstream ss(line);
        std::string type;

        if (!(ss >> type) || type[0] == '#') {
            continue;
        }

        try {
            if (type == "v") {
                float x, y, z;

                if (!(ss >> x >> y >> z)) {
                    throw std::runtime_error(
                        "Invalid vertex position"
                    );
                }

                data.vertices.emplace_back(x, y, z);
            }
            else if (type == "vn") {
                float x, y, z;

                if (!(ss >> x >> y >> z)) {
                    throw std::runtime_error(
                        "Invalid vertex normal"
                    );
                }

                data.normals.emplace_back(x, y, z);
            }
            else if (type == "vt") {
                float u;
                float v = 0.0f;

                if (!(ss >> u)) {
                    throw std::runtime_error(
                        "Invalid texture coordinate"
                    );
                }

                // A second coordinate is common but optional in OBJ.
                ss >> v;

                data.texCoords.emplace_back(u, v);
            }
            else if (type == "f") {
                std::vector<OBJVertexIndex> face;
                std::string token;

                while (ss >> token) {
                    // Ignore an inline comment.
                    if (token[0] == '#') {
                        break;
                    }

                    face.push_back(parseFaceVertex(token, data));
                }

                if (face.size() < 3) {
                    throw std::runtime_error(
                        "A face must have at least 3 vertices"
                    );
                }

                // Triangulate as a fan:
                // (0,1,2), (0,2,3), (0,3,4), ...
                //
                // This is suitable for triangles and convex polygons.
                for (std::size_t i = 1; i + 1 < face.size(); ++i) {
                    const OBJVertexIndex& a = face[0];
                    const OBJVertexIndex& b = face[i];
                    const OBJVertexIndex& c = face[i + 1];

                    data.triangles.emplace_back(
                        data.vertices[a.vertex],
                        data.vertices[b.vertex],
                        data.vertices[c.vertex],
                        mat
                    );

                    data.triangleIndices.push_back({
                        {a, b, c}
                    });
                }
            }

            // Other OBJ records, such as o, g, s, usemtl and mtllib,
            // are currently ignored.
        }
        catch (const std::exception& e) {
            std::cerr << "OBJ parse error at line "
                      << lineNumber << ": "
                      << e.what() << '\n';

            return {};
        }
    }

    if (file.bad()) {
        std::cerr << "Error while reading OBJ: "
                  << filename << '\n';
        return {};
    }

    if (data.vertices.empty()) {
        std::cerr << "OBJ contains no vertex positions: "
                  << filename << '\n';
        return {};
    }

    return data;
}

int parseOBJIndex(int index, std::size_t count) {
    if (index == 0) {
        throw std::runtime_error("OBJ index cannot be zero");
    }

    // This function returns int, so reject arrays too large
    // to represent with that return type.
    if (count >
        static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::runtime_error("OBJ attribute array is too large");
    }

    const std::int64_t resolved =
        index > 0
            ? static_cast<std::int64_t>(index) - 1
            : static_cast<std::int64_t>(count) + index;

    if (resolved < 0 ||
        resolved >= static_cast<std::int64_t>(count)) {
        throw std::runtime_error("Invalid OBJ index");
    }

    return static_cast<int>(resolved);
}
