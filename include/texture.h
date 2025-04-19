#ifndef TEXURE_H
#define TEXURE_H

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stb_image.h>

enum ETexType { RED, RGB, RGBA, SRGB, SRGBA };

class Texture2D {
   public:
    Texture2D(const std::string& path, const std::string typeName, ETexType type = RGBA) : path(path), texType(type), typeName(typeName) {
        isValid = LoadTextureFromFile(path, type);
    }
    Texture2D(const char* path, const std::string typeName, ETexType type = RGBA) : Texture2D(std::string(path), typeName, type) {}
    virtual ~Texture2D() { DeleteTexture2D(); }

    void        DeleteTexture2D() { glDeleteTextures(1, &this->textureID); }
    GLuint      textureID;
    std::string path;
    std::string name;
    std::string typeName;  // 纹理贴图的属性(法线，高光，高度...)
    bool        isValid;

   private:
    bool LoadTextureFromFile(const std::string& path, const ETexType type) {
        int         width, height, nrComponents;
        std::string path_s = path;

        std::replace(path_s.begin(), path_s.end(), '\\', '/');

        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        glGenTextures(1, &this->textureID);
        glBindTexture(GL_TEXTURE_2D, this->textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data != nullptr) {
            this->path       = path_s;
            this->width      = width;
            this->height     = height;
            this->nrChannels = nrComponents;

            // sRGB和RGB显示方式
            if (nrChannels == 1) {
                texType = ETexType::RED;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            } else if (nrChannels == 3) {
                if (type == ETexType::SRGB || type == ETexType::SRGBA) {
                    texType = ETexType::SRGB;
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                } else {
                    texType = ETexType::RGB;
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                }
            } else if (nrChannels == 4) {
                if (type == ETexType::SRGB || type == ETexType::SRGBA) {
                    texType = ETexType::SRGBA;
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                } else {
                    texType = ETexType::RGBA;
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                }
            }
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);

            name = path_s.substr(path_s.find_last_of('/') + 1, path_s.size());
        } else {
            std::cerr << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);
            return false;
        }
        return true;
    }

    int      width;
    int      height;
    int      nrChannels;
    ETexType texType;
};

#endif