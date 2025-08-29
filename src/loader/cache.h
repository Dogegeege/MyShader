#pragma once
#include "global.h"
#include "singleton_util.h"
#include "source.h"

namespace engine {

/**
 * @brief 从加载到Cache中的数据读取到已经保存过的source，将新的source 缓存到这个类里面。
 *source在被其他对象放弃使用权的时候广播，并且没有被引用的时候析构
 */
class Cache : public Singleton<Cache> {
   public:
    static Cache* getInstance();

    Cache() noexcept;

    ~Cache() noexcept;
    /**
     * @brief 从缓存中获得source
     * @param path 资源唯一标识符，可以是硬盘路径，网络url，嵌入式资源名称
     * @return 返回资源指针，没有找到返回`nullptr`
     */
    Source::ptr getSource(const std::string& path) noexcept;

    /**
     * @brief 缓存新的source
     * @param path 资源唯一标识符，可以是硬盘路径，网络url，嵌入式资源名称
     * @param source 新的资源指针
     */
    void cacheSource(const std::string& path, Source::ptr source) noexcept;

    /// \brief 监听
    /// \param e
    // auto onSourceRelease(const EventBase::ptr& e) -> void;

   private:
    // static Cache* mInstance;

    /// hashType = size_t
    std::unordered_map<HashType, Source::ptr> mSources{};

    std::mutex mMutex;
};
}  // namespace engine
