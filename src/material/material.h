#pragma once

#include "global.h"
#include "identity.h"

namespace engine {

/**
 * @brief 用于检查材质类型
 */
class MaterialTypeChecker {
   public:
    bool mIsMaterial          = false;
    bool mIsMeshBasicMaterial = false;
    bool mIsMeshPhongMaterial = false;
    bool mIsLineBasicMaterial = false;
    bool mIsCubeMaterial      = false;
    bool mIsDepthMaterial     = false;
};

/**
 * @brief 所有材质的基类
 */
class Material : public MaterialTypeChecker {
   public:
    using ptr = std::shared_ptr<Material>;
    static ptr create() { return std::make_shared<Material>(); }

    Material() noexcept;

    ~Material() noexcept;

    inline std::string getType() const noexcept { return mType; }
    inline ID          getID() const noexcept { return mID; }

   public:
    bool     mNeedsUpdate{true};  // 传递给backeng是否需要更新材质参数
    uint32_t mVersion{1};         // version 用于首次解析

    /// raster
    FrontFace mFrontFace{FrontFace::FrontCounterClockWise};
    Side      mSide{Side::DoubleSide};
    DrawMode  mDrawMode{DrawMode::Triangles};

    /// blending
    bool             mTransparent{false};  /// 控制是否开启blending
    float            mOpacity{1.0f};       /// 透明度，越小，越透明
    BlendingType     mBlendingType{BlendingType::CustomBlending};
    BlendingFactor   mBlendSrc{BlendingFactor::SrcAlpha};
    BlendingFactor   mBlendDst{BlendingFactor::OneMinusSrcAlpha};
    BlendingEquation mBlendEquation{BlendingEquation::AddEquation};
    BlendingFactor   mBlendSrcAlpha{BlendingFactor::None};
    BlendingFactor   mBlendDstAlpha{BlendingFactor::Zero};
    BlendingEquation mBlendEquationAlpha{BlendingEquation::AddEquation};

    /// depth
    bool            mDepthTest{true};   /// 是否开启深度检测
    bool            mDepthWrite{true};  /// 是否通过测试后更新覆盖深度
    CompareFunction mDepthFunction{CompareFunction::LessOrEqual};
    double          mDepthClearColor{1.0};

    /// diffuse
    // Texture::ptr mDiffuseMap{nullptr};

    /// envMap
    // CubeTexture::ptr mEnvMap{nullptr};

    /// normalMap
    // Texture::ptr mNormalMap{nullptr};

    /// specularMap
    // Texture::ptr mSpecularMap{nullptr};

   protected:
    ID          mID{0};
    std::string mType;
};
}  // namespace engine