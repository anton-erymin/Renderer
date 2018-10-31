#pragma once

#include <functional>

#include "Types.h"
#include "Math.h"
#include "Texture.h"

/* Rasterization algorithms. */

using TriangleFillCallback = std::function<void(int32_t, int32_t, int32_t, int32_t, bool, bool)>;
using TriangleLineCallback = std::function<void(int32_t, int32_t, size_t)>;
using LineCallback = std::function<void(int32_t, int32_t)>;

void LineDDA(const Vec2i &x1, const Vec2i &x2, LineCallback &&callback);
void LineBresenham(const Vec2i & x1, const Vec2i & x2, LineCallback && callback);
void RasterizeTrianglePoint(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback &&callback);
void RasterizeTriangleLine(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback && callback);
void RasterizeTriangleFill(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleFillCallback &&callback);

/* Postprocessing algorithms. */

void GammaCorrection(Texture &target, float_t gamma);
void ReinhardTonemapping(Texture &target);
void Blur(Texture &target, size_t iterations = 1);
void GaussianBlur(Texture &target, const std::vector<float_t> &kernel, size_t iterations);