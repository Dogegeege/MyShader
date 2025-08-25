#include "source.h"

namespace engine {
Source::ptr Source::create() {
    return std::make_shared<Source>();
}

Source::Source() noexcept = default;

Source::~Source() noexcept = default;
}  // namespace engine
