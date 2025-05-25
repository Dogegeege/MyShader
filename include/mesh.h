#ifndef MESH_H
#define MESH_H

#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

#define MAX_BONE_INFLUENCE 4

/**
 * @brief
 * 介于C++结构体的特性,下列元素的内存地址是连续存放的，当它读入Buffer时,每个顶点的元素都会依次读入
 * @param Position 顶点坐标
 * @param Normal 法线方向
 * @param TexCoords UV坐标
 * @param Tangent
 * @param Bitangent
 */
struct Vertex {
    glm::vec3 Position;   // 顶点位置
    glm::vec3 Normal;     // 法线方向
    glm::vec2 TexCoords;  // UV坐标
    glm::vec3 Tangent;    // tangent
    glm::vec3 Bitangent;  // bitangent

    int m_BoneIDs[MAX_BONE_INFLUENCE];  // bone indexes which will influence this
                                        // vertex

    float m_Weights[MAX_BONE_INFLUENCE];  // weights from each bone
};

class Mesh {
   public:
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<std::shared_ptr<Texture2D>> textures);
    void Draw(Shader& shader);

    /*  网格数据  */
    std::vector<Vertex>                     vertices;
    std::vector<GLuint>                     indices;
    std::vector<std::shared_ptr<Texture2D>> textures;

   private:
    void SetupMesh();

    GLuint VAO, VBO, EBO;
};

#endif