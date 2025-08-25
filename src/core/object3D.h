#pragma once
#include "global.h"
#include "identity.h"

namespace engine {
class ObjectTypeChecker {
   public:
    bool mIsRenderableObject{false};
    bool mIsMesh{false};
    bool mIsSkinnedMesh{false};
    bool mIsBone{false};
    bool mIsScene{false};
    bool mIsCamera{false};
    bool mIsPerpectiveCamera{false};
    bool mIsOrthographicCamera{false};
    bool mIsGroup{false};
    bool mIsLight{false};
    bool mIsAmbientLight{false};
    bool mIsDirectionalLight{false};
};

/**
 * @brief Object3D是所有空间变换、节点结构等的最基础的类型
 */
class Object3D : public std::enable_shared_from_this<Object3D>, public ObjectTypeChecker {
   public:
    /// 使用智能指针进行管理
    using ptr = std::shared_ptr<Object3D>;
    static ptr create();

    Object3D() noexcept;
    virtual ~Object3D() noexcept;

    inline virtual void fakeFunction() noexcept {};  // 用于动态类型转换

    void setPosition(float x, float y, float z) noexcept;
    void setPosition(const glm::vec3& position) noexcept;
    void setQuaternion(float x, float y, float z, float w) noexcept;
    void setScale(float x, float y, float z) noexcept;

    /**
     * @brief 绕模型坐标系的X轴旋转
     * @param angle 角度制
     */
    void rotateX(float angle) noexcept;

    /**
     * @brief 绕模型坐标系的Y轴旋转
     * @param angle 角度制
     */
    void rotateY(float angle) noexcept;

    /**
     * @brief 绕模型坐标系的Z轴旋转
     * @param angle 角度制
     */
    void rotateZ(float angle) noexcept;

    /**
     * @brief 绕模型坐标系的任意轴旋转
     * @param axis 任意轴方向
     * @param angle 角度制
     */
    void rotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

    /**
     * @brief 重置角度，绕模型坐标系的任意轴重新旋转
     * @param axis 任意轴方向
     * @param angle 角度制
     */
    void setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

    /**
     * @brief 设置Object3D看向 `target` 的方向，更新自己模型坐标系的旋转部分
     * @param target 看向的位置
     * @param up 世界坐标系的up
     */
    void lookAt(const glm::vec3& target, const glm::vec3& up = worldUP) noexcept;

    /**
     *  @brief 设置模型坐标系（相对于父节点的局部坐标系）
     *  @param localMatrix 模型坐标变换矩阵
     */
    void setLocalMatrix(const glm::mat4& localMatrix) noexcept;

    /**
     *  @brief 设置模型的世界坐标系
     *  @param worldMatrix 世界坐标变换矩阵
     */
    void setWorldMatrix(const glm::mat4& worldMatrix) noexcept;

    /**
     * @brief 向当前的Object3D加入子节点
     * @param child 子节点
     * @return 若待添加节点不为空，或者不为该节点本身，则返回`true`
     */
    bool addChild(const Object3D::ptr& child) noexcept;

    /// @brief 更新Object3D 的模型坐标系
    virtual void updateMatrix() noexcept;

    /**
     * @brief 更新 Object3D 的世界坐标系，每一层的父节点都是子节点的世界坐标。
     * 如果更新全部节点，则更新完成后所有的节点的 `worldMatrix` 都将局部坐映射到根节点的世界坐标。
     * 若只有单级节点，则世界坐标等于模型坐标（即将模型坐标直接映射到世界坐标中）
     * @param updateParent 如果有父节点，那么需要回溯父节点的 `worldMatrix`
     * @param updateChildren 递归更新子节点的 `worldMatrix`
     * @return 祖先节点`worldMatrix`的累乘
     */
    virtual glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept;

    /**
     * @brief 更新Object3D 的 ModelView 矩阵
     * @param viewMatrix 摄像机 view 矩阵
     * @return 模型+视图变换矩阵（摄像机坐标系）
     */
    glm::mat4 updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept;

    /// @brief 更新Object3D 的normal矩阵，该矩阵将法线由局部模型坐标，映射到View变换后的世界坐标
    glm::mat3 updateNormalMatrix() noexcept;

    inline glm::vec3 getPosition() const noexcept { return glm::vec3(mLocalMatrix[3]); }       // 局部坐标
    inline glm::vec3 getWorldPosition() const noexcept { return glm::vec3(mWorldMatrix[3]); }  // 世界坐标
    inline glm::vec3 getLocalDirection() const noexcept { return glm::normalize(-glm::vec3(mLocalMatrix[2])); }
    inline glm::vec3 getWorldDirection() const noexcept { return glm::normalize(-glm::vec3(mWorldMatrix[2])); }
    inline glm::vec3 getUp() const noexcept { return glm::normalize(glm::vec3(mLocalMatrix[1])); }     // 模型坐标系的up(y轴 头顶)方向
    inline glm::vec3 getRight() const noexcept { return glm::normalize(glm::vec3(mLocalMatrix[0])); }  // 模型坐标系的x轴(右手)方向
    inline glm::mat4 getLocalMatrix() const noexcept { return mLocalMatrix; }
    inline glm::mat4 getWorldMatrix() const noexcept { return mWorldMatrix; }
    inline glm::mat4 getModelViewMatrix() const noexcept { return mModelViewMatrix; }
    inline glm::mat3 getNormalMatrix() const noexcept { return mNormalMatrix; }
    inline const std::vector<Object3D::ptr>& getChildren() const noexcept { return mChildren; }
    inline ID                                getID() const noexcept { return mID; }

   protected:
    /**
     * @brief 分解变换矩阵，将变换矩阵分解为平移、旋转、缩放等基本变换分量，在每次更新模型坐标 `mLocalMatrix` 参数时调用
     */
    void decompose() noexcept;

   public:
    bool mVisible{true};            // visible来表示是否对其进行渲染
    bool mCastShadow{true};         // 是否产生阴影
    bool mNeedsUpdateMatrix{true};  // 表示是否强制对矩阵进行更新

    std::string mName;

   protected:
    ID mID{0};

    glm::vec3 mPosition{glm::vec3(0.0f)};                      // 局部坐标
    glm::quat mQuaternion{glm::quat(1.0f, 0.0f, 0.0f, 0.0f)};  // 旋转四元数
    glm::vec3 mScale{glm::vec3(1.0f)};                         // 缩放
    glm::mat4 mLocalMatrix = glm::mat4(1.0f);                  // 局部坐标系
    glm::mat4 mWorldMatrix = glm::mat4(1.0f);                  // 世界坐标系

    /// 保留参数
    bool mNeedsUpdate{false};

    // 节点系统
    std::weak_ptr<Object3D>    mParent;      // 父节点采用weakPtr ，防止循环引用
    std::vector<Object3D::ptr> mChildren{};  // 父节点存储了子节点的sharedPtr，建立一次引用,保证子节点的引用计数至少大于1

    // for shading
    // modelViewMatrix将模型顶点，从模型坐标系，转换到当前摄像机坐标系,viewMatrix * worldMatrix
    glm::mat4 mModelViewMatrix = glm::mat4(1.0f);

    /// 将模型的normal从模型坐标系，转换到世界坐标系
    glm::mat3 mNormalMatrix = glm::mat3(1.0f);
};

}  // namespace engine
