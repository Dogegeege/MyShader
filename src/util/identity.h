#pragma once

#include "global.h"

namespace engine {

class Identity {
   public:
    static ID generateID() { return ++mCurrentID; }

   private:
    static ID mCurrentID;
};

}  // namespace engine