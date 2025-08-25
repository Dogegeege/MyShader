#include "object3D.h"

namespace engine {
Object3D::ptr Object3D::create() {
    return std::make_shared<Object3D>();
}

Object3D::Object3D() noexcept {
    mID = Identity::generateID();
}

Object3D::~Object3D() noexcept {
    // EventBase::ptr e = EventBase::create("objectDispose");
    // e->mTarget       = this;
    // EventDispatcher::getInstance()->dispatchEvent(e);
}

void Object3D::setPosition(float x, float y, float z) noexcept {
    setPosition(glm::vec3(x, y, z));
}

void Object3D::setPosition(const glm::vec3& position) noexcept {
    /// 列优先
    mLocalMatrix[3].x = position.x;
    mLocalMatrix[3].y = position.y;
    mLocalMatrix[3].z = position.z;

    mPosition = position;
}

void Object3D::setQuaternion(float x, float y, float z, float w) noexcept {
    /// 在四元数情况下，glm的初始化，w xyz
    glm::quat quaternion(w, x, y, z);

    float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
    float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
    float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));

    glm::mat4 rotateMatrix = glm::mat4_cast(quaternion);

    /// 恢复scale变换
    mLocalMatrix[0] = rotateMatrix[0] * scaleX;
    mLocalMatrix[1] = rotateMatrix[1] * scaleY;
    mLocalMatrix[2] = rotateMatrix[2] * scaleX;

    decompose();
}

void Object3D::setScale(float x, float y, float z) noexcept {
    auto col0 = glm::normalize(glm::vec3(mLocalMatrix[0])) * x;
    auto col1 = glm::normalize(glm::vec3(mLocalMatrix[1])) * y;
    auto col2 = glm::normalize(glm::vec3(mLocalMatrix[2])) * z;

    mLocalMatrix[0] = glm::vec4(col0, 0.0f);
    mLocalMatrix[1] = glm::vec4(col1, 0.0f);
    mLocalMatrix[2] = glm::vec4(col2, 0.0f);

    decompose();
}

void Object3D::rotateX(float angle) noexcept {
    /// 当前模型状态下的右侧方向
    glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[0]);
    mLocalMatrix         = glm::rotate(mLocalMatrix, glm::radians(angle), rotateAxis);

    decompose();
}

void Object3D::rotateY(float angle) noexcept {
    glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[1]);
    mLocalMatrix         = glm::rotate(mLocalMatrix, glm::radians(angle), rotateAxis);

    decompose();
}

void Object3D::rotateZ(float angle) noexcept {
    glm::vec3 rotateAxis = glm::vec3(mLocalMatrix[2]);
    mLocalMatrix         = glm::rotate(mLocalMatrix, glm::radians(angle), rotateAxis);

    decompose();
}

void Object3D::rotateAroundAxis(const glm::vec3& axis, float angle) noexcept {
    mLocalMatrix = glm::rotate(mLocalMatrix, glm::radians(angle), axis);

    decompose();
}

void Object3D::setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept {
    glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0), glm::radians(angle), axis);

    // decompose scale
    float     scaleX      = glm::length(glm::vec3(mLocalMatrix[0]));
    float     scaleY      = glm::length(glm::vec3(mLocalMatrix[1]));
    float     scaleZ      = glm::length(glm::vec3(mLocalMatrix[2]));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scaleX, scaleY, scaleZ));

    // 列相等
    mLocalMatrix[0] = rotateMatrix[0];
    mLocalMatrix[1] = rotateMatrix[1];
    mLocalMatrix[2] = rotateMatrix[2];

    mLocalMatrix *= scaleMatrix;

    decompose();
}

void Object3D::lookAt(const glm::vec3& target, const glm::vec3& up) noexcept {
    /// decompose scale
    float scaleX = glm::length(glm::vec3(mLocalMatrix[0]));
    float scaleY = glm::length(glm::vec3(mLocalMatrix[1]));
    float scaleZ = glm::length(glm::vec3(mLocalMatrix[2]));

    glm::vec3 position = glm::vec3(mLocalMatrix[3]);

    /// make local coordinate
    auto nTarget = glm::normalize(target - position) * scaleZ;
    auto nRight  = glm::normalize(glm::cross(up, -nTarget)) * scaleX;
    auto nUp     = glm::normalize(glm::cross(nRight, nTarget)) * scaleY;

    mLocalMatrix[0] = glm::vec4(nRight, 0.0f);
    mLocalMatrix[1] = glm::vec4(nUp, 0.0f);
    mLocalMatrix[2] = glm::vec4(-nTarget, 0.0f);
    mLocalMatrix[3] = glm::vec4(position, 1.0f);

    decompose();
}

void Object3D::setLocalMatrix(const glm::mat4& localMatrix) noexcept {
    mLocalMatrix = localMatrix;

    decompose();
}

void Object3D::setWorldMatrix(const glm::mat4& worldMatrix) noexcept {
    mWorldMatrix = worldMatrix;
}

void Object3D::updateMatrix() noexcept {
    if (mNeedsUpdateMatrix = true) {
        mNeedsUpdateMatrix   = false;
        auto translateMatrix = glm::translate(glm::mat4(1.0f), mPosition);
        auto rotateMatrix    = glm::mat4_cast(mQuaternion);
        auto scaleMatrix     = glm::scale(glm::mat4(1.0f), mScale);

        mLocalMatrix = translateMatrix * rotateMatrix * scaleMatrix;
    }
}

glm::mat4 Object3D::updateWorldMatrix(bool updateParent, bool updateChildren) noexcept {
    // 检查有没有父节点
    if (mParent.expired() == false && updateParent == true) {  // 递归更新父节点
        const auto parent = mParent.lock();
        parent->updateWorldMatrix(true, false);
    }

    updateMatrix();

    mWorldMatrix = mLocalMatrix;

    // 如果有父节点,需要左乘父节点变换矩阵
    if (mParent.expired() == false) {
        auto parent  = mParent.lock();
        mWorldMatrix = parent->mWorldMatrix * mWorldMatrix;
    }

    // 依次更新子节点的worldMatrix
    if (updateChildren == true) {
        for (auto& child : mChildren) { child->updateWorldMatrix(false, true); }
    }

    return mWorldMatrix;
}

glm::mat4 Object3D::updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept {
    mModelViewMatrix = viewMatrix * mWorldMatrix;

    return mModelViewMatrix;
}

glm::mat3 Object3D::updateNormalMatrix() noexcept {
    /// normalMatrix 由于存在scale的影响，不能直接变换，否则normal会无法保证垂直于表面
    mNormalMatrix = glm::transpose(glm::inverse(glm::mat3(mModelViewMatrix)));

    return mNormalMatrix;
}

bool Object3D::addChild(const Object3D::ptr& child) noexcept {
    if (child == shared_from_this()) { return false; }

    child->mParent = shared_from_this();

    auto iter = std::find(mChildren.begin(), mChildren.end(), child);
    if (iter != mChildren.end()) { return false; }

    mChildren.push_back(child);

    return true;
}

void Object3D::decompose() noexcept {
    glm::vec3 skew;
    glm::vec4 perspective;

    // 将变换矩阵 mLocalMatrix 分解为平移（mPosition）、旋转（mQuaternion）、缩放（mScale）等基本变换分量。
    glm::decompose(mLocalMatrix, mScale, mQuaternion, mPosition, skew, perspective);
}

}  // namespace engine