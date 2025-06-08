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

    virtual void         Draw(Shader& shader) = 0;
    virtual void         SetID(unsigned int& id) { id = ID; }
    virtual unsigned int GetID() const { return ID; }
    virtual void         SetName(std::string& name) { name = this->name; }
    virtual std::string  GetName() const { return name; }

   private:
    unsigned int ID;  // OpenGL对象ID
    std::string  name;
};

class Object3D : public Object {
   public:
    Object3D() : Object(0, " ") {}
    Object3D(unsigned int id, const std::string& name) : Object(id, name) {}
    virtual ~Object3D() { loadedObject3D.erase(this->GetID()); };

    virtual void Draw(Shader& shader) = 0;
    virtual void Draw(Shader& shader, Shader& highLightShader) {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);  // 每一位都可以被修改，即启用模板缓冲写入
        Draw(shader);

        // 外层轮廓
        if (IsHighLight() == true) {
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

    virtual void                     SetModelMatrix(const glm::mat4& model) { modelMatrix = model; }
    virtual glm::mat4                GetModelMatrix() const { return modelMatrix; }
    virtual glm::mat4*               GetModelMatrixPointer() { return &modelMatrix; }
    virtual void                     SetHighLight(bool isHighLight) { this->isHighLight = isHighLight; }
    virtual bool                     IsHighLight() const { return isHighLight; }
    static std::shared_ptr<Object3D> GetObject3D(const unsigned int ID) {
        auto it = loadedObject3D.find(ID);
        if (it != loadedObject3D.end()) { return it->second; }
        return nullptr;
    }

    static std::map<unsigned int, std::shared_ptr<Object3D>> loadedObject3D;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 translate   = glm::vec3(0.0f);
    glm::vec3 rotate      = glm::vec3(0.0f);  // 角度制
    float     scale       = 1.0f;

   private:
    bool isHighLight = false;  // 是否启用背景轮廓
};

#endif  // OBJECT_H