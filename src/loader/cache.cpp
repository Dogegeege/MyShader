#include "cache.h"

namespace engine {
//*已在singleton_util.h中实现

// Cache* Cache::mInstance = nullptr;

// Cache* Cache::getInstance() {
//     static std::once_flag oneFlag;
//     std::call_once(oneFlag, []() { mInstance = new Cache(); });

//     return mInstance;
// }

Cache::Cache() noexcept {
    // EventDispatcher::getInstance()->addEventListener("sourceRelease", this, &Cache::onSourceRelease);
}

Cache::~Cache() noexcept = default;

Source::ptr Cache::getSource(const std::string& path) noexcept {
    constexpr std::hash<std::string> hasher;
    const auto                       hashCode = hasher(path);

    // std::lock_guard<std::mutex> lock(mMutex);
    Source::ptr source = nullptr;

    if (const auto iter = mSources.find(hashCode); iter != mSources.end()) {
        source = iter->second;
        source->mRefCount++;
    }

    return source;
}

void Cache::cacheSource(const std::string& path, Source::ptr source) noexcept {
    constexpr std::hash<std::string> hasher;
    auto                             hashCode = hasher(path);

    //  std::lock_guard<std::mutex> lock(mMutex);

    /// 寻找是否重复
    if (const auto iter = mSources.find(hashCode); iter != mSources.end()) { return; }

    /// 没找到同名同姓的source
    source->mHashCode = hashCode;

    /// 每次只要生成source，就一定会有一个texture来使用他，refCount就必然+1
    /// Todo：我们可以将refCount做成private，friend Cache，就会造成只能在cache里访问
    source->mRefCount++;

    mSources.insert(std::make_pair(hashCode, source));
}

/// cache会监听sourceRelease
// auto Cache::onSourceRelease(const EventBase::Ptr& e) -> void {
//     const auto source   = static_cast<Source*>(e->mTarget);
//     const auto hashCode = source->mHashCode;

//     std::lock_guard<std::mutex> lock(mMutex);  /// 使用互斥锁保护共享数据

//     const auto iter = mSources.find(hashCode);
//     if (iter == mSources.end()) { return; }

//     /// 如果确实存在在cache里面，则引用计数-1
//     source->mRefCount--;

//     /// 如果引用计数 = 0，说明整个程序没有人再使用这个source，那么就从cache里清除
//     if (source->mRefCount == 0) {
//         /// source通过sharedPtr管理，那么再erase之后，并且以前持有他的对象都不再持有，
//         /// source这个智能指针的引用计数变为了0，则会构成source的析构
//         mSources.erase(iter);
//     }
// }
}  // namespace engine
