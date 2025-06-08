#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

#include "object.h"
#include "texture.h"

class Skybox : public Object3D {
   public:
    Skybox(const std::vector<std::string>& skyboxFaces, std::string name = "Skybox") : Object3D(), skyboxTexture(skyboxFaces, "skybox") {
        this->SetName(name);
        SetupSkybox(name);
    };
    ~Skybox() {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
        skyboxTexture.DeleteTexture();
    };

    void Draw(Shader& shader) override {
        shader.use();
        if (skyboxTexture.IsValid() == false) {
            std::cerr << "Skybox texture is not valid!" << std::endl;
            return;
        }
        shader.use();
        shader.setUnsignedInt("skybox", 0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.GetID());
        // 使用立方体贴图...

        // skybox cube
        glBindVertexArray(skyboxVAO);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.GetID());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    };

   private:
    void SetupSkybox(const std::string& path) {
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);

        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    };

    unsigned int   skyboxVAO, skyboxVBO;
    TextureCubeMap skyboxTexture;
    float          skyboxVertices[108] = {
        // positions
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

        1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
};

#endif  // SKYBOX_H