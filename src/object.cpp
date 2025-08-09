#include "object.h"

template <typename T, typename U>
auto multiply(T x, U y) -> decltype(x * y) {
    return x * y;
}

void fin() {
    multiply(1, 2);
}