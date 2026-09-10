#include <parser/objParser.hpp>
#include <fstream>
#include <sstream>
#include <core/material.hpp>
#include <iostream>

OBJData OBJParser::parser(const std::string& filename) {

    std::ifstream file(filename);
    if (!file) {
        std::cout << "Can't open file";
    }

    Material mat;
    // Temporary Color before i add feature to extract material from .obj
    mat.albedo = Color(200, 50, 50);

    std::string line;

    while (std::getline(file, line)) {

        std::stringstream ss(line);

        char type;
        ss >> type;

        if (type == 'v') {

            float x, y, z;
            ss >> x >> y >> z;

            data.vertices.push_back(Point3(x, y, z));

        } else if (type == 'f') {

            std::vector<int> indices;
            std::string vertex;

            while (ss >> vertex) {

                size_t slash = vertex.find('/');

                int index = std::stoi(vertex.substr(0, slash));

                indices.push_back(index - 1);
            }

            for (size_t i = 1; i + 1 < indices.size(); i++) {

                data.triangles.push_back(
                    Triangle(
                        data.vertices[indices[0]],
                        data.vertices[indices[i]],
                        data.vertices[indices[i + 1]],
                        mat
                    )
                );
            }
        }
    }

    return data;
}