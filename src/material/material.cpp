#include "material.h"
#include "GLconstant.h"

namespace engine {

Material::Material() noexcept {  // 默认构造
    mID         = Identity::generateID();
    mType       = MaterialName::Material;
    mIsMaterial = true;
}

Material::~Material() noexcept {
    /// 再析构的时候，向外发出消息，通知监听函数们，哪一个材质消亡了
    // EventBase::ptr e = EventBase::create("materialDispose");
    // e->mTarget       = this;
    // EventDispatcher::getInstance()->dispatchEvent(e);
}

}  // namespace engine