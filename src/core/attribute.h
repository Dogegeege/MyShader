#pragma once

#include "global.h"
#include "identity.h"

namespace engine {

/**
 * @tparam T 数据类型
 * @brief Attribute 类用于创建专有内存区用于存储顶点属性数据，如位置、颜色等。
 */
template <typename T>
class Attribute : public std::enable_shared_from_this<Attribute<T>> {
   public:
    using ptr = std::shared_ptr<Attribute<T>>;
    static ptr create(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType = BufferAllocType::StaticDrawBuffer) {
        return std::make_shared<Attribute<T>>(data, itemSize, bufferAllocType);
    }
    /**
     * @tparam T 数据类型
     * @param data 顶点属性数据
     * @param itemSize 每个顶点占用的的字节大小
     * @param bufferAllocType 缓冲区分配类型，默认为静态绘制缓冲区(读入显存)
     */
    Attribute(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType = BufferAllocType::StaticDrawBuffer) noexcept;
    ~Attribute() noexcept;

    void setX(const uint32_t& index, T value) noexcept;
    void setY(const uint32_t& index, T value) noexcept;
    void setZ(const uint32_t& index, T value) noexcept;
    T    getX(const uint32_t& index) noexcept;
    T    getY(const uint32_t& index) noexcept;
    T    getZ(const uint32_t& index) noexcept;

    inline auto  getID() const noexcept { return mID; }
    inline auto  getData() const noexcept { return mData; }
    inline auto  getCount() const noexcept { return mCount; }
    inline auto  getItemSize() const noexcept { return mItemSize; }
    inline bool  getNeedsUpdate() const noexcept { return mNeedsUpdate; }
    inline void  clearNeedsUpdate() noexcept { mNeedsUpdate = false; }
    inline void  setNeedsUpdate() noexcept { mNeedsUpdate = true; }
    inline auto  getBufferAllocType() const noexcept { return mBufferAllocType; }
    inline Range getUpdateRange() const noexcept { return mUpdateRange; }
    inline void  clearUpdateRange() noexcept {
        mUpdateRange.mOffset = 0;
        mUpdateRange.mCount  = -1;
    }
    inline auto getDataType() const noexcept { return mDataType; }

   private:
    ID              mID{0};
    uint32_t        mItemSize{0};  // 一个顶点的字节位
    uint32_t        mCount{0};     // 顶点数量
    BufferAllocType mBufferAllocType{BufferAllocType::StaticDrawBuffer};
    DataType        mDataType{DataType::FloatType};

    bool mNeedsUpdate{true};

    std::vector<T> mData{};
    Range          mUpdateRange{};
};

using Attributef = Attribute<float>;
using Attributei = Attribute<uint32_t>;

template <typename T>
Attribute<T>::Attribute(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType) noexcept {
    mID = Identity::generateID();

    mData            = data;
    mItemSize        = itemSize;
    mCount           = static_cast<uint32_t>(mData.size()) / itemSize;
    mBufferAllocType = bufferAllocType;

    mDataType = toDataType<T>();
}

template <typename T>
Attribute<T>::~Attribute() noexcept {
    // 发送消息，给到各类监听的函数，告诉他们，哪个attribute已经消亡了
    // EventBase::Ptr e = EventBase::create("attributeDispose");
    // e->mTarget       = this;
    // e->mpUserData    = &mID;

    // EventDispatcher::getInstance()->dispatchEvent(e);
}

template <typename T>
auto Attribute<T>::setX(const uint32_t& index, T value) noexcept -> void {
    assert(index < mCount);
    mData[index * mItemSize] = value;
    mNeedsUpdate             = true;
}

template <typename T>
auto Attribute<T>::setY(const uint32_t& index, T value) noexcept -> void {
    assert(index < mCount);
    mData[index * mItemSize + 1] = value;
    mNeedsUpdate                 = true;
}

template <typename T>
auto Attribute<T>::setZ(const uint32_t& index, T value) noexcept -> void {
    assert(index < mCount);
    mData[index * mItemSize + 2] = value;
    mNeedsUpdate                 = true;
}

template <typename T>
auto Attribute<T>::getX(const uint32_t& index) noexcept -> T {
    assert(index < mCount);
    return mData[index * mItemSize];
}

template <typename T>
auto Attribute<T>::getY(const uint32_t& index) noexcept -> T {
    assert(index < mCount);
    return mData[index * mItemSize + 1];
}

template <typename T>
auto Attribute<T>::getZ(const uint32_t& index) noexcept -> T {
    assert(index < mCount);
    return mData[index * mItemSize + 2];
}

}  // namespace engine
