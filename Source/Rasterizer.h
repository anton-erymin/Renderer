#pragma once

#include <functional>

#include "Types.h"
#include "Texture.h"

constexpr size_t RASTERIZER_MAX_ATTRIBUTES = 16;

enum class PolygonMode {
    Fill,
    Line,
    Point
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

// Side 1 from v1 - v3
// Side 2 from v1 - v2
// Side 3 from v2 - v3

class Rasterizer {
public:
    Rasterizer();
    void DrawTriangle(DrawTriangleInfo &triangleInfo, Texture &target);

private:
    struct AttributeInterpolationInfo {
        // For each attribute we have 3 its own slope interpolation coeffs for each triangle side
        float_t d1;
        float_t d2;
        float_t d3;
        float_t left;
        float_t right;
        float_t dhor;
        float_t attrValue;
    };

    using TriangleFillCallback = std::function<void(int32_t, int32_t, int32_t, int32_t, bool, bool)>;
    using TriangleLineCallback = std::function<void(int32_t, int32_t, size_t)>;
    using LineCallback         = std::function<void(int32_t, int32_t)>;

    void RasterizeTriangleFill(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleFillCallback &&callback) const;
    void RasterizeTriangleLine(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback &&callback) const;
    void RasterizeTrianglePoint(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback &&callback) const;

    void LineDDA(const Vec2i &x1, const Vec2i &x2, LineCallback &&callback) const;
    void LineBresenham(const Vec2i &x1, const Vec2i &x2, LineCallback &&callback) const;

    AttributeInterpolationInfo attrs_[RASTERIZER_MAX_ATTRIBUTES];
};

