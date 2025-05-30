#ifndef TEXURE_H
#define TEXURE_H

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stb_image.h>

enum ETexType { RED, RGB, RGBA, SRGB, SRGBA };

class Texture {
   public:
    Texture(const std::string& typeName) : typeName(typeName), isValid(false), textureID(0) {}
    virtual ~Texture() { DeleteTexture(); }

    inline void DeleteTexture() {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
    }

    // 公有接口
    unsigned int GetID() const { return textureID; }
    std::string  GetPath() const { return path; }
    std::string  GetName() const { return name; }
    std::string  GetTypeName() const { return typeName; }
    bool         IsValid() const { return isValid; }

   protected:
    virtual bool LoadTexture() = 0;  // 纯虚函数，强制子类实现加载逻辑

    unsigned int textureID;
    std::string  path;
    std::string  name;
    std::string  typeName;  // 纹理贴图的属性(法线，高光，高度...)
    bool         isValid;
};

class Texture2D : public Texture {
   public:
    Texture2D(const std::string& path, const std::string& typeName, ETexType type = RGBA)
        : Texture(typeName), texType(type), nrChannels(0), width(0), height(0) {
        std::string processedPath = path;
        std::replace(processedPath.begin(), processedPath.end(), '\\', '/');
        this->path    = processedPath;
        this->isValid = LoadTexture();
    }
    Texture2D(const char* path, const std::string& typeName, ETexType type = RGBA) : Texture2D(std::string(path), typeName, type) {}

    ETexType GetTexType() const { return texType; }
    int      GetWidth() const { return width; }
    int      GetHeight() const { return height; }

   private:
    bool LoadTexture() override {
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cerr << "Texture2D failed to load: " << path << std::endl;
            return false;
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 根据通道数和类型决定格式
        GLenum format         = GL_RGB;
        GLenum internalFormat = GL_RGB;
        switch (nrChannels) {
            case 1:
                internalFormat = GL_RED;
                format         = GL_RED;
                texType        = RED;
                break;
            case 3:
                internalFormat = (texType == SRGB) ? GL_SRGB : GL_RGB;
                format         = GL_RGB;
                break;
            case 4:
                internalFormat = (texType == SRGBA) ? GL_SRGB_ALPHA : GL_RGBA;
                format         = GL_RGBA;
                break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);

        // 提取文件名
        size_t lastSlash = path.find_last_of('/');
        name             = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;

        return true;
    }

    int      width;
    int      height;
    int      nrChannels;
    ETexType texType;
};

class TextureCubeMap : public Texture {
   public:
    TextureCubeMap(const std::vector<std::string>& faces, const std::string& typeName) : Texture(typeName) {
        for (const auto& face : faces) {
            std::string processedFace = face;
            std::replace(processedFace.begin(), processedFace.end(), '\\', '/');
            this->faces.push_back(processedFace);
        }
        this->isValid = LoadTexture();
    }

   private:
    bool LoadTexture() override {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < this->faces.size(); i++) {
            unsigned char* data = stbi_load(this->faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (!data) {
                std::cerr << "Cubemap texture failed to load: " << this->faces[i] << std::endl;
                return false;
            }

            GLenum format = GL_RGB;
            switch (nrChannels) {
                case 1: format = GL_RED; break;
                case 3: format = GL_RGB; break;
                case 4: format = GL_RGBA; break;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            // 加载完立即释放
            stbi_image_free(data);
        }

        // 设置立方体贴图参数
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        // 记录第一个面的路径作为主路径
        if (!this->faces.empty()) {
            path             = this->faces[0];
            size_t lastSlash = path.find_last_of('/');
            name             = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
        }

        return true;
    }

    std::vector<std::string> faces;
    std::vector<std::string> paths;
};

#endif