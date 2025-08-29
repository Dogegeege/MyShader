#pragma once
#include "global.h"
#include "source.h"

namespace engine {

/// 负责给出纹理贴图描述信息
class Texture {
   public:
    using ptr = std::shared_ptr<Texture>;
    static ptr create(const uint32_t&        width,
                      const uint32_t&        height,
                      const DataType&        dataType  = DataType::UnsignedByteType,
                      const TextureWrapping& wrapS     = TextureWrapping::RepeatWrapping,
                      const TextureWrapping& wrapT     = TextureWrapping::RepeatWrapping,
                      const TextureWrapping& wrapR     = TextureWrapping::RepeatWrapping,
                      const TextureFilter&   magFilter = TextureFilter::LinearFilter,
                      const TextureFilter&   minFilter = TextureFilter::NearestFilter,
                      const TextureFormat&   format    = TextureFormat::RGBA) {
        return std::make_shared<Texture>(width, height, dataType, wrapS, wrapT, wrapR, magFilter, minFilter, format);
    }

    Texture(const uint32_t&        width,
            const uint32_t&        height,
            const DataType&        dataType  = DataType::UnsignedByteType,
            const TextureWrapping& wrapS     = TextureWrapping::RepeatWrapping,
            const TextureWrapping& wrapT     = TextureWrapping::RepeatWrapping,
            const TextureWrapping& wrapR     = TextureWrapping::RepeatWrapping,
            const TextureFilter&   magFilter = TextureFilter::LinearFilter,
            const TextureFilter&   minFilter = TextureFilter::LinearFilter,
            const TextureFormat&   format    = TextureFormat::RGBA) noexcept;

    virtual ~Texture() noexcept;

    inline ID           getID() const noexcept { return mID; }
    inline TextureUsage getUsage() const noexcept { return mUsage; }

    virtual Texture::ptr clone() noexcept;

   public:
    /// \brief
    /// Filter，Nearest四舍五入， 直接取得某个像素的颜色值
    /// Filter，Linear取得周边像素的颜色值，按照远近程度标定权重，加权平均
    TextureFilter mMinFilter{TextureFilter::LinearFilter};
    TextureFilter mMagFilter{TextureFilter::LinearFilter};

    /// \brief  环绕方式，纹理坐标超出了0-1处理方式
    TextureWrapping mWrapS{TextureWrapping::RepeatWrapping};
    TextureWrapping mWrapT{TextureWrapping::RepeatWrapping};
    TextureWrapping mWrapR{TextureWrapping::RepeatWrapping};

    /// @brief 像素格式信息
    TextureFormat mFormat{TextureFormat::RGBA};           // 即输入的图片像素格式
    TextureFormat mInternalFormat{TextureFormat::RGBA};   // 在shader采样的时候，内存中存放的格式
    DataType      mDataType{DataType::UnsignedByteType};  // 每个颜色通道的数值格式
    uint32_t      mWidth{0};                              // 宽
    uint32_t      mHeight{0};                             // 高

    /// @brief 原图片数据指针
    Source::ptr mSource{nullptr};

    bool mNeedsUpdate{true};

    TextureType mTextureType{TextureType::Texture2D};  // 纹理类型（平面纹理，天空盒体贴图/纹理数组..)

    TextureUsage mUsage{TextureUsage::SamplerTexture};  // 纹理用处（贴图，画布（colorRendertarget/colorAttachment））

   protected:
    ID mID{0};
};
}  // namespace engine