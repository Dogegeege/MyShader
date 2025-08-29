#pragma once

#include <assert.h>
#include <math.h>
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace engine {

using byte     = unsigned char;
using HashType = size_t;

using ID = uint32_t;

struct Range {
    int32_t mOffset{0};
    int32_t mCount{-1};
};

enum class DataType { UnsignedByteType, ByteType, Int32Type, UInt32Type, FloatType };

static size_t toSize(const DataType& value) {
    switch (value) {
        case DataType::UnsignedByteType: return sizeof(unsigned char);
        case DataType::FloatType: return sizeof(float);
        case DataType::ByteType: return sizeof(char);
        case DataType::Int32Type: return sizeof(int);
        case DataType::UInt32Type: return sizeof(uint32_t);
        default: return 0;
    }
}

template <typename T>
static auto toDataType() -> DataType {
    if (typeid(T) == typeid(float)) { return DataType::FloatType; }
    if (typeid(T) == typeid(int)) { return DataType::Int32Type; }
    if (typeid(T) == typeid(char)) { return DataType::ByteType; }
    if (typeid(T) == typeid(unsigned char)) { return DataType::UnsignedByteType; }
    if (typeid(T) == typeid(uint32_t)) { return DataType::UInt32Type; }
    return DataType::FloatType;
}

static constexpr glm::vec3 worldUP = glm::vec3(0.0f, 1.0f, 0.0f);

// data
enum class BufferAllocType { StaticDrawBuffer, DynamicDrawBuffer };

// keyboard

#define DOWN 0
#define UP   1

static constexpr size_t MAX_KEY_NUM = 108;

using KeyBoardState   = std::bitset<MAX_KEY_NUM>;
using KeyboardAction  = uint64_t;
using KeyboardMapType = std::unordered_map<uint32_t, KeyboardAction>;

static constexpr KeyboardAction WKey     = 1;
static constexpr KeyboardAction SKey     = 2;
static constexpr KeyboardAction AKey     = 3;
static constexpr KeyboardAction DKey     = 4;
static constexpr KeyboardAction SPACEKey = 5;
static constexpr KeyboardAction SHIFTKey = 6;
static constexpr KeyboardAction KeyNONE  = 31;

// mouse
enum class MouseAction : uint8_t {
    LeftDown,
    RightDown,
    MiddleDown,
    LeftUp,
    RightUp,
    MiddleUp,
    NONE,
};

// texture
enum class TextureType { Texture2D, TextureCubeMap };
enum class TextureFormat { RGB, RGBA, DepthFormat, DepthStencilFormat };
enum class TextureWrapping {
    RepeatWrapping,
    ClampToEdgeWrapping,
    ClampToBorder,
    MirroredRepeatWrapping,
};
enum class TextureFilter {
    LinearFilter,
    NearestFilter,
};
enum class TextureUsage {
    SamplerTexture,      // << 贴图使用（ 采样的纹理 ）
    RenderTargetTexture  // << 渲染目标，用于颜色(ID)附件
};

// material
enum class BlendingType { DefaultBlending, CustomBlending, NoBlending, None };
enum class BlendingFactor { Zero, One, SrcAlpha, OneMinusSrcAlpha, None };
enum class BlendingEquation { AddEquation, MinEquation, MaxEquation, None };

namespace MaterialName {
static const std::string Material          = "Material";
static const std::string MeshBasicMatirial = "MeshBasicMatirial";
static const std::string MeshPhongMaterial = "MeshPhongMaterial";
static const std::string CubeMaterial      = "CubeMaterial";
static const std::string DepthMaterial     = "DepthMaterial";
}  // namespace MaterialName

// draw
enum class DrawMode { Lines, LinesStrip, Triangles, TrianglesStrip, Points, LineLoop, Quads, QuadsStrip, None };
enum class Side { FrontSide, BackSide, DoubleSide, None };
enum class FrontFace { FrontClockWise, FrontCounterClockWise, None };
enum class CompareFunction { Less, LessOrEqual, Greater, GreaterOrEqual, None };

}  // namespace engine