#ifndef TEXURE_H
#define TEXURE_H

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

class texure {
   public:
    texure(const char* texPath, const unsigned int GL_TEXURE_TYPE, const bool flip = false) : width(width), height(height), nrChannels(nrChannels) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);  // 绑定纹理对象
        glActiveTexture(GL_TEXURE_TYPE);          // 激活纹理单元
        glBindTexture(GL_TEXTURE_2D, textureID);  // 绑定纹理对象

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // 纹理超出边界环绕模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // 纹理被缩小的时候使用MipMap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                // 被放大时使用线性过滤（模糊）

        // 载入时是否翻转图片
        if (flip) {
            stbi_set_flip_vertically_on_load(true);
        } else {
            stbi_set_flip_vertically_on_load(false);
        }

        data = stbi_load(texPath, &width, &height, &nrChannels, 0);
        if (data != NULL) {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture:\n" << std::endl;
        }
        stbi_image_free(data);
    };

   private:
    int            width, height, nrChannels;
    unsigned char* data;
    unsigned int   textureID;
};

#endif