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

#include "object.h"

class Line : public Object {};

/**
 * !Grid 改用`Line`
 */
class Grid : public Object {
   public:
    Grid() { SetupGrid(); }
    void Draw(Shader& shader) override;

   private:
    void SetupGrid();

    float axis[6][6] = {
        {100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, {-100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

        {0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, -100.0f, 0.0f, 0.0f, 1.0f, 0.0f},

        {0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -100.0f, 0.0f, 0.0f, 1.0f},
    };

    unsigned int VAO;
    unsigned int VBO;
};

#endif