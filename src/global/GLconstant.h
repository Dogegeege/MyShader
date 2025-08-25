#pragma once

#include "global.h"

namespace engine {

static unsigned int toGL(const DataType& value) {
    switch (value) {
        case DataType::UnsignedByteType: return GL_UNSIGNED_BYTE;
        case DataType::FloatType: return GL_FLOAT;
        case DataType::ByteType: return GL_BYTE;
        case DataType::Int32Type: return GL_INT;
        case DataType::UInt32Type: return GL_UNSIGNED_INT;
        default: std::cerr << "Faild to translate DataType" << std::endl; return 0;
    }
}

static uint32_t toGL(const BufferAllocType& type) {
    switch (type) {
        case BufferAllocType::StaticDrawBuffer: return GL_STATIC_DRAW;
        case BufferAllocType::DynamicDrawBuffer: return GL_DYNAMIC_DRAW;
        default: return 0;
    }
}

static const KeyboardMapType KeyboardActionMap = {
    KeyboardMapType::value_type(GLFW_KEY_W, WKey),         KeyboardMapType::value_type(GLFW_KEY_S, SKey),
    KeyboardMapType::value_type(GLFW_KEY_A, AKey),         KeyboardMapType::value_type(GLFW_KEY_D, DKey),
    KeyboardMapType::value_type(GLFW_KEY_SPACE, SPACEKey), KeyboardMapType::value_type(GLFW_KEY_LEFT_SHIFT, SHIFTKey),
};

// texture
static constexpr uint32_t MAX_TEXUTRE_COUNT = 8;
static constexpr GLuint   MAX_TEXTURE       = GL_TEXTURE0 + 7;

static GLuint toGL(const TextureType& value) noexcept {
    switch (value) {
        case TextureType::Texture2D: return GL_TEXTURE_2D;
        case TextureType::TextureCubeMap: return GL_TEXTURE_CUBE_MAP;
        default: return GL_NONE;
    }
}

static GLuint toGL(const TextureFormat& format) noexcept {
    switch (format) {
        case TextureFormat::RGB: return GL_RGB;
        case TextureFormat::RGBA: return GL_RGBA;
        case TextureFormat::DepthFormat: return GL_DEPTH_COMPONENT32F;
        case TextureFormat::DepthStencilFormat: return GL_DEPTH24_STENCIL8;
        default: return 0;
    }
}

static uint32_t toPixelSize(const TextureFormat& format) {
    switch (format) {
        case TextureFormat::RGB: return 24;
        case TextureFormat::RGBA: return 32;
        default: return 0;
    }
}

static uint32_t toByteSize(const TextureFormat& format) {
    switch (format) {
        case TextureFormat::RGB: return 3;
        case TextureFormat::RGBA: return 4;
        default: return 0;
    }
}

static uint32_t toGL(const Side& side) {
    switch (side) {
        case Side::FrontSide: return GL_BACK;
        case Side::BackSide: return GL_FRONT;
        default: return GL_BACK;
    }
}

static uint32_t toGL(const FrontFace& value) {
    switch (value) {
        case FrontFace::FrontCounterClockWise: return GL_CCW;
        case FrontFace::FrontClockWise: return GL_CW;
        default: return GL_NONE;
    }
}

static uint32_t toGL(const BlendingFactor& value) {
    switch (value) {
        case BlendingFactor::SrcAlpha: return GL_SRC_ALPHA;
        case BlendingFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case BlendingFactor::One: return GL_ONE;
        case BlendingFactor::Zero: return GL_ZERO;
        default: return GL_NONE;
    }
}

static uint32_t toGL(const CompareFunction& value) {
    switch (value) {
        case CompareFunction::Less: return GL_LESS;
        case CompareFunction::Greater: return GL_GREATER;
        case CompareFunction::LessOrEqual: return GL_LEQUAL;
        case CompareFunction::GreaterOrEqual: return GL_GEQUAL;
        default: return GL_NONE;
    }
}

static uint32_t toGL(const DrawMode& value) {
    switch (value) {
        case DrawMode::Lines: return GL_LINES;
        case DrawMode::LinesStrip: return GL_LINE_STRIP;
        case DrawMode::Triangles: return GL_TRIANGLES;
        case DrawMode::TrianglesStrip: return GL_TRIANGLE_STRIP;
        case DrawMode::Points: return GL_POINTS;
        case DrawMode::LineLoop: return GL_LINE_LOOP;
        case DrawMode::Quads: return GL_QUADS;
        default: return GL_NONE;
    }
}

}  // namespace engine