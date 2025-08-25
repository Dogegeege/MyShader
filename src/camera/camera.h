#pragma once
#include "object3D.h"

namespace engine {

class Camera : public Object3D {
   public:
    using ptr = std::shared_ptr<Camera>;

    Camera() noexcept;
    ~Camera() noexcept;

    glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept override;

    glm::mat4 getWorldMatrixInverse() const noexcept { return mWorldMatrixInverse; }
    glm::mat4 getProjectionMatrix() const noexcept { return mProjectionMatrix; }

    /// @brief 更新摄像机的投影矩阵
    virtual glm::mat4 updateProjectionMatrix() noexcept = 0;

   protected:
    glm::mat4 mWorldMatrixInverse = glm::mat4(1.0f);  // 摄像机的 `view` 矩阵（相当于映射到世界坐标的逆矩阵）
    glm::mat4 mProjectionMatrix   = glm::mat4(1.0f);  // 摄像机的投影矩阵（正交/透视，由子类定义）
};

}  // namespace engine