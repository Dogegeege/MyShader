#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <memory>
#include <string>

#include "shader.h"

class Object {
   public:
    Object() : ID(0), name(" ") {}
    Object(unsigned int id, const std::string& name) : ID(id), name(name) {}
    virtual ~Object() {};

    virtual void Draw(Shader& shader) = 0;
    virtual void GetID(unsigned int& id) const { id = ID; }
    virtual void GetName(std::string& name) const { name = this->name; }

    unsigned int ID;  // OpenGL对象ID
    std::string  name;
};

class Object3D : public Object {
   public:
    Object3D() : Object(0, " ") {}
    Object3D(unsigned int id, const std::string& name) : Object(id, name) {}
    virtual ~Object3D() { loadedObject3D.erase(name); };

    virtual void Draw(Shader& shader) = 0;
    virtual void Draw(Shader& shader, Shader& highLightShader) {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);  // 每一位都可以被修改，即启用模板缓冲写入
        Draw(shader);

        // 外层轮廓
        if (isHighLight == true) {
            //?如果禁用深度测试,则外层轮廓呈现透视状态
            // glDisable(GL_DEPTH_TEST);
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);  // 禁止修改

            Draw(highLightShader);
            //  glEnable(GL_DEPTH_TEST);
        }

        // *写回保证下一轮glClear可以清除模板缓存
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
    }

    static std::shared_ptr<Object3D> GetObject3D(const std::string& name) {
        auto it = loadedObject3D.find(name);
        if (it != loadedObject3D.end()) { return it->second; }
        return nullptr;
    }

    virtual void      SetModelMatrix(const glm::mat4& model) { modelMatrix = model; }
    virtual glm::mat4 GetModelMatrix() const { return modelMatrix; }
    virtual void      SetHighLight(bool isHighLight) { this->isHighLight = isHighLight; }
    virtual bool      IsHighLight() const { return isHighLight; }

    static std::map<std::string, std::shared_ptr<Object3D>> loadedObject3D;

   private:
    bool      isHighLight = false;  // 是否启用背景轮廓
    glm::mat4 modelMatrix = glm::mat4(1.0f);
};

#endif  // OBJECT_H