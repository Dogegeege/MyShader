#ifndef TEXURE_H
#define TEXURE_H

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

class Texure {
   public:
    Texure(const char* texPath, const unsigned int texureID, const bool flip = false) : texureID(texureID) {
        glGenTextures(1, &texure);

        // 载入时是否翻转图片
        if (flip) {
            stbi_set_flip_vertically_on_load(true);
        } else {
            stbi_set_flip_vertically_on_load(false);
        }

        data = stbi_load(texPath, &width, &height, &nrChannels, 0);
        if (data != NULL) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texure);                                                      // 绑定纹理对象
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);  // 生成纹理

            glActiveTexture(static_cast<unsigned int>(GL_TEXTURE0 + texureID));  // 激活纹理单元
            glBindTexture(GL_TEXTURE_2D, texure);                                // 绑定纹理对象

            glGenerateMipmap(GL_TEXTURE_2D);

            // 纹理超出边界环绕模式
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // 纹理被缩小的时候使用MipMap
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                // 被放大时使用线性过滤（模糊）

        } else {
            std::cerr << "Failed to load texture:\n" << std::endl;
        }
        stbi_image_free(data);
    };

    inline unsigned int getTexureObject() { return texure; }

   private:
    int            width, height, nrChannels;
    unsigned char* data;
    unsigned int   texure;    // 纹理对象指针
    unsigned int   texureID;  // 纹理单元ID
};

#endif