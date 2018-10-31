#pragma once

#include <functional>

#include "Types.h"
#include "Texture.h"
#include "Shader.h"

constexpr size_t RASTERIZER_MAX_ATTRIBUTES = 64;

enum class PolygonMode {
    Fill = 0,
    Line = 1,
    Point = 2
};

struct RasterizerVertex {
    Vec2i   screenCoords;
    float_t *attributes;
};

struct DrawTriangleInfo {
    RasterizerVertex v1;
    RasterizerVertex v2;
    RasterizerVertex v3;
    size_t           numAttributes;
    PolygonMode      polygonMode;
};

class Rasterizer {
public:
    Rasterizer();
    void DrawTriangle(DrawTriangleInfo &triangleInfo, Texture &target, Texture *depthTarget, PhongFragmentShader &shader);

private:
    struct AttributeInterpolationInfo {
        // For each attribute we have 3 its own slope interpolation coeffs for each triangle side
        float_t d1;
        float_t d2;
        float_t d3;
        float_t left;
        float_t right;
        float_t dhor;
        float_t attributeValue;
    };

    AttributeInterpolationInfo attributeInfos[RASTERIZER_MAX_ATTRIBUTES];
};

