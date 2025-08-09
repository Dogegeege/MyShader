#pragma once

#include "attribute.h"
#include "global.h"

namespace engine {

class Box3 {
   public:
    /// 使用智能指针包装
    using ptr = std::shared_ptr<Box3>;
    static ptr create() { return std::make_shared<Box3>(); }

    Box3() noexcept  = default;
    ~Box3() noexcept = default;

    inline void setFromAttribute(const Attributef::ptr& attribute) noexcept {
        for (uint32_t i = 0; i < attribute->getCount(); ++i) {
            float x = attribute->getX(i);
            float y = attribute->getY(i);
            float z = attribute->getZ(i);

            if (x < mMin.x) { mMin.x = x; }
            if (y < mMin.y) { mMin.y = y; }
            if (z < mMin.z) { mMin.z = z; }

            if (x > mMax.x) { mMax.x = x; }
            if (y > mMax.y) { mMax.y = y; }
            if (z > mMax.z) { mMax.z = z; }
        }
    }

    inline bool isEmpty() noexcept { return (mMax.x < mMin.x || mMax.y < mMin.y || mMax.z < mMin.z); }

    inline glm::vec3 getCenter() noexcept {
        if (isEmpty() == true) { return glm::vec3(0.0f); }
        return (mMax + mMin) / 2.0f;
    }

   public:
    // min是包围盒xyz最小的那个点,max是包围盒xyz最大的那个点
    glm::vec3 mMin = glm::vec3(std::numeric_limits<float>::infinity());
    glm::vec3 mMax = glm::vec3(-std::numeric_limits<float>::infinity());
};

class SphereBox3 {
   public:
    using Ptr = std::shared_ptr<SphereBox3>;
    static Ptr create(const glm::vec3& center, float radius) { return std::make_shared<SphereBox3>(center, radius); }

    SphereBox3(const glm::vec3& center, float radius) noexcept {
        mCenter = center;
        mRadius = radius;
    }

    ~SphereBox3() noexcept = default;

    /// \brief 应用在跟随物体进行matrix变换的时候
    /// \param matrix
    auto applyMatrix4(const glm::mat4 matrix) noexcept -> void {
        mCenter = glm::vec3(matrix * glm::vec4(mCenter, 1.0));

        /// 对于半径，只会收到scale缩放影响，我们只需要考虑三个Scale当中最大的哪个
        float scaleX = glm::length(glm::vec3(matrix[0]));
        float scaleY = glm::length(glm::vec3(matrix[1]));
        float scaleZ = glm::length(glm::vec3(matrix[2]));

        float maxScale = std::max(std::max(scaleX, scaleY), scaleZ);
        mRadius *= maxScale;
    }

    /// \brief
    /// 举例：
    /// Sphere::Ptr s1 = Sphere::create(xxx);
    /// Sphere::Ptr s2 = s1;
    /// s2->mRadius = 10.0f;
    /// 这样做的话，就会导致s1指向的内存，也会发生变化
    /// 应该：
    /// Sphere::Ptr s2 = Sphere::create(xxx);
    /// s2->copy(s1);
    /// \param other
    auto copy(const SphereBox3::Ptr& other) -> void {
        mCenter = other->mCenter;
        mRadius = other->mRadius;
    }

   public:
    glm::vec3 mCenter = glm::vec3(0.0f);
    float     mRadius{0.0f};
};

}  // namespace engine