#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "shader.h"

class Object3D {
   public:
    Object3D() = default;
    virtual ~Object3D() { loadedObject3D.erase(name); };

    virtual void Draw(Shader& shader) = 0;

    std::string name;
    glm::mat4   modelMatrix = glm::mat4(1.0f);

    static std::map<std::string, std::shared_ptr<Object3D>> loadedObject3D;
    static std::shared_ptr<Object3D>                        GetObject3D(const std::string& name) {
        auto it = loadedObject3D.find(name);
        if (it != loadedObject3D.end()) { return it->second; }
        return nullptr;
    }
};

#endif  // OBJECT_H