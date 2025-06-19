#ifndef SHAPE_H
#define SHAPE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>  // 包含glad来获取所有的必须OpenGL头文件
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "object.h"

struct Vertex8f {
    Vertex8f(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) : Position(position), TexCoords(texCoords), Normal(normal) {};
    glm::vec3 Position;   // 顶点位置
    glm::vec3 Normal;     // 法线方向
    glm::vec2 TexCoords;  // UV坐标
};

class Line : public Object {};

class Rectangle : public Object3D {
   public:
    Rectangle(const unsigned int id, const std::string& name) : Object3D(id, name) {
        vertices.push_back({{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
        vertices.push_back({{0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}});
        vertices.push_back({{0.5f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}});
        vertices.push_back({{-0.5f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}});
        SetupRectangle();
    }
    Rectangle() : Rectangle(0, "Rectangle") {}

    void Draw(Shader& shader) override;

   private:
    void SetupRectangle();

    std::vector<Vertex8f> vertices;
    unsigned int          VAO;
    unsigned int          VBO;
};

/**
 * !重写
 * !Grid 改用`Line`
 */
class Grid : public Object {
   public:
    Grid() { SetupGrid(); }
    void Draw(Shader& shader) override;

   private:
    void SetupGrid();

    float axis[6][6] = {
        {1000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, {-1000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

        {0.0f, 1000.0f, 0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, -1000.0f, 0.0f, 0.0f, 1.0f, 0.0f},

        {0.0f, 0.0f, 1000.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1000.0f, 0.0f, 0.0f, 1.0f},
    };

    unsigned int VAO;
    unsigned int VBO;
};

#endif