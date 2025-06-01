#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures)
    : vertices(vertices), indices(indices), textures(textures) {
    SetupMesh();
}

void Mesh::Draw(Shader& shader) {
    //! 注意，从1开始计数

    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    //*一般不用管循环，因为一个网格对应一个纹理
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);  // 在绑定之前激活相应的纹理单元

        // 获取纹理序号（diffuse_textureN 中的 N）

        std::string number;
        std::string name = textures[i]->GetTypeName();
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);  // 漫反射材质纹理
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);  // 镜面反射材质纹理
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);  // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++);  // transfer unsigned int to string

        // 需要保证着色器内部格式统一
        //* 比如 material.texure_diffuse1
        shader.setUnsignedInt((name + number), i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->GetID());
    }
    // if (textures.empty() == true) std::cerr << "mssing textures  " << this->VAO << std::endl;

    // 绘制网格
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    //?接下来这些参数 面切线..暂时未用到-----------------------------------------------------------
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // // ids
    // glEnableVertexAttribArray(5);
    // glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // // weights
    // glEnableVertexAttribArray(6);
    // glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

    //?----------------------------------------------------------------------------------------------
    glBindVertexArray(0);
}
