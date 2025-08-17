#pragma once

#include <assert.h>
#include <math.h>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace engine {

using byte     = unsigned char;
using HashType = size_t;

static constexpr size_t MAX_KEY_NUM = 108;
using KeyBoardState                 = std::bitset<MAX_KEY_NUM>;

using ID = uint32_t;

struct Range {
    int32_t mOffset{0};
    int32_t mCount{-1};
};

enum class DataType { UnsignedByteType, ByteType, Int32Type, UInt32Type, FloatType };

template <typename T>
static auto toDataType() -> DataType {
    if (typeid(T) == typeid(float)) { return DataType::FloatType; }
    if (typeid(T) == typeid(int)) { return DataType::Int32Type; }
    if (typeid(T) == typeid(char)) { return DataType::ByteType; }
    if (typeid(T) == typeid(unsigned char)) { return DataType::UnsignedByteType; }
    if (typeid(T) == typeid(uint32_t)) { return DataType::UInt32Type; }
    return DataType::FloatType;
}

enum class BufferAllocType { StaticDrawBuffer, DynamicDrawBuffer };

static auto toGL(const BufferAllocType& type) -> uint32_t {
    switch (type) {
        case BufferAllocType::StaticDrawBuffer: return GL_STATIC_DRAW;
        case BufferAllocType::DynamicDrawBuffer: return GL_DYNAMIC_DRAW;
        default: return 0;
    }
}

}  // namespace engine