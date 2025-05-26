#ifndef GRID_H
#define GRID_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>  // 包含glad来获取所有的必须OpenGL头文件
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class Grid {
   public:
    Grid() { SetupGrid(); }
    void Draw(Shader& shader) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 6);  // 绘制三条轴线
        glBindVertexArray(0);
    }

   private:
    void SetupGrid() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);

        // 顶点位置
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
    }

    float axis[6][6] = {
        {100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, {-100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

        {0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, -100.0f, 0.0f, 0.0f, 1.0f, 0.0f},

        {0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -100.0f, 0.0f, 0.0f, 1.0f},
    };

    unsigned int VAO;
    unsigned int VBO;
};

#endif