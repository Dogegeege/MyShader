#include "texture.h"
#include "identity.h"

namespace engine {
Texture::Texture(const uint32_t&        width,
                 const uint32_t&        height,
                 const DataType&        dataType,
                 const TextureWrapping& wrapS,
                 const TextureWrapping& wrapT,
                 const TextureWrapping& wrapR,
                 const TextureFilter&   magFilter,
                 const TextureFilter&   minFilter,
                 const TextureFormat&   format) noexcept
    : mID(Identity::generateID()),
      mWidth(width),
      mHeight(height),
      mDataType(dataType),
      mWrapS(wrapS),
      mWrapR(wrapR),
      mWrapT(wrapT),
      mMagFilter(magFilter),
      mMinFilter(minFilter),
      mFormat(format),
      mTextureType(TextureType::Texture2D) {}

Texture::~Texture() noexcept {
    // /// 消亡的时候，通过dispatcher向外发出本texture消亡的消息
    // EventBase::ptr e = EventBase::create("textureDispose");
    // e->mTarget       = this;
    // EventDispatcher::getInstance()->dispatchEvent(e);

    // if (mSource) {
    //     EventBase::ptr e = EventBase::create("sourceRelease");
    //     e->mTarget       = mSource.get();
    //     EventDispatcher::getInstance()->dispatchEvent(e);
    // }
}

Texture::ptr Texture::clone() noexcept {
    auto texture             = Texture::create(mWidth, mHeight, mDataType, mWrapS, mWrapT, mWrapR, mMagFilter, mMinFilter, mFormat);
    texture->mSource         = mSource;
    texture->mUsage          = mUsage;
    texture->mTextureType    = mTextureType;
    texture->mInternalFormat = mInternalFormat;

    return texture;
}
}  // namespace engine
