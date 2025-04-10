#ifndef MODEL_H
#define MODEL_H

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

class Model {
   public:
    Model(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& prop, const int fragSize = 3)
        : vertices(vertices), indices(indices), prop(prop), fragSize(fragSize) {}

    Model(const std::string verticesFile, const std::string indicesFile, const std::string propFile, const int fragSize = 3);

    Model(const int fragSize = 3);

    std::vector<float>        vertices;
    std::vector<unsigned int> indices;  // 绘制三角形,三个索引一组
    std::vector<unsigned int> prop;
    int                       fragSize;
};

template <typename T>
void csvModLoader(const std::string filePath, std::vector<T>& container) {
    // .csv
    std::ifstream verticesStream(filePath);
    if (!verticesStream.is_open()) {
        std::cerr << "Error: Could not open " << filePath << std::endl;
        return;
    }

    std::string line;
    bool        isHeader = true;
    while (std::getline(verticesStream, line)) {
        if (isHeader == true) {
            isHeader = false;  // 跳过表头
            continue;
        }

        std::istringstream ss(line);
        std::string        value;

        while (std::getline(ss, value, ',')) {
            if constexpr (std::is_same<T, float>::value) {
                container.push_back(std::stof(value));  // 转换为 float
            } else if constexpr (std::is_same<T, unsigned int>::value) {
                container.push_back(std::stoul(value));  // 转换为 unsigned int
            } else {
                static_assert("Unsupported type for csvModLoader");
            }
        }
    }
    verticesStream.close();
}

#endif