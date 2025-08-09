#pragma once

#include "attribute.h"
#include "bbox.h"
#include "global.h"

namespace engine {

class Geometry : public std::enable_shared_from_this<Geometry> {
   public:
    using AttributeMap = std::unordered_map<std::string, Attributef::ptr>;
    using ptr          = std::shared_ptr<Geometry>;
    static ptr create() { return std::make_shared<Geometry>(); }

    Geometry() noexcept;
    ~Geometry() noexcept;

    void            setAttribute(const std::string& name, Attributef::ptr attribute) noexcept;
    Attributef::ptr getAttribute(const std::string& name) noexcept;
    void            deleteAttribute(const std::string& name) noexcept;
    bool            hasAttribute(const std::string& name) noexcept;
    void            computeBoundingBox() noexcept;
    void            computeBoundingSphere() noexcept;

    inline void                setIndex(const Attributei::ptr& index) noexcept { mIndexAttribute = index; }
    inline const AttributeMap& getAttributes() const noexcept { return mAttributes; }
    inline ID                  getID() const noexcept { return mID; };
    inline auto                getIndex() const noexcept { return mIndexAttribute; }
    inline Box3::ptr           getBoundingBox() const noexcept { return mBoundingBox; }

    // inline Sphere::Ptr getBoundingSphere() const noexcept { return mBoundingSphere; }

   protected:
    ID              mID;
    AttributeMap    mAttributes{};
    Attributei::ptr mIndexAttribute{nullptr};  // index的Attribute单独存放，并没有加到map里面

    Box3::ptr mBoundingBox{nullptr};  // 包围盒
    // Sphere::ptr mBoundingSphere{nullptr};  // 包围球
};

}  // namespace engine