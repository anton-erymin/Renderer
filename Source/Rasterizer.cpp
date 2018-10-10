#include "Rasterizer.h"

#include <stdexcept>

#include "Logger.h"

Rasterizer::Rasterizer() {
    std::memset(attrs_, 0, RASTERIZER_MAX_ATTRIBUTES * sizeof(AttributeInterpolationInfo));
}

void Rasterizer::DrawTriangle(DrawTriangleInfo &triangleInfo, Texture &target) {
    if (triangleInfo.numAttributes > RASTERIZER_MAX_ATTRIBUTES) {
        throw std::runtime_error{"Exceeds maximum number of vertex attributes"};
    }

    Vec2i &x1 = triangleInfo.v1.screenCoords;
    Vec2i &x2 = triangleInfo.v2.screenCoords;
    Vec2i &x3 = triangleInfo.v3.screenCoords;

    // Sort vertices
    if (x1.y > x2.y) {
        std::swap(triangleInfo.v1, triangleInfo.v2);
    }
    if (x1.y > x3.y) {
        std::swap(triangleInfo.v1, triangleInfo.v3);
    }
    if (x2.y > x3.y) {
        std::swap(triangleInfo.v2, triangleInfo.v3);
    }

    // Degenerated triangle
    if (x1.y == x3.y) {
        return;
    }
    
    const float_t sideHeight1 = float_t(x3.y - x1.y);
    const float_t sideHeight2 = float_t(x2.y - x1.y);
    const float_t sideHeight3 = float_t(x3.y - x2.y);

    for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
        attrs_[i].d1 = float_t(triangleInfo.v3.attributes[i] - triangleInfo.v1.attributes[i]) / sideHeight1;
        attrs_[i].d2 = sideHeight2 > 0 ? float_t(triangleInfo.v2.attributes[i] - triangleInfo.v1.attributes[i]) / sideHeight2 : 0;
        attrs_[i].d3 = sideHeight3 > 0 ? float_t(triangleInfo.v3.attributes[i] - triangleInfo.v2.attributes[i]) / sideHeight3 : 0;
    }

    switch (triangleInfo.polygonMode) {
    case PolygonMode::Fill: {
        // Just to make sure y is not equal to first pixel y coordinate
        int32_t ycur = x1.y - 1;
        RasterizeTriangleFill(x1, x2, x3,
            [&](int32_t x, int32_t y, int32_t xl, int32_t xr, bool secondHalf, bool swapped) {
            if (ycur != y) {
                ycur = y;
                // Changed to the next row so it is needed to calc new interpolated values on both sides of that row
                // and along the row too
                for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                    attrs_[i].left  = triangleInfo.v1.attributes[i] + attrs_[i].d1 * (y - x1.y);
                    attrs_[i].right = secondHalf ? triangleInfo.v2.attributes[i] + attrs_[i].d3 * (y - x2.y)
                                                 : triangleInfo.v1.attributes[i] + attrs_[i].d2 * (y - x1.y);
                    if (swapped) {
                        std::swap(attrs_[i].left, attrs_[i].right);
                    }
                }
                // dattrHor will contain interpolation coeffs along the horizontal line
                for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                    attrs_[i].dhor = (attrs_[i].right - attrs_[i].left) / float_t(xr - xl);
                }
            }

            for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                attrs_[i].attrValue = attrs_[i].left + attrs_[i].dhor * float_t(x - xl);
            }
            target.Set(x, y, Color3{attrs_[0].attrValue, attrs_[1].attrValue, attrs_[2].attrValue});

        });
        break;
    }
    case PolygonMode::Line:
        RasterizeTriangleLine(x1, x2, x3, [&](int32_t x, int32_t y, size_t side) {
            for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                switch (side) {
                case 1:
                    attrs_[i].attrValue = triangleInfo.v1.attributes[i] + attrs_[i].d1 * (y - x1.y);
                    break;
                case 2:
                    attrs_[i].attrValue = triangleInfo.v1.attributes[i] + attrs_[i].d2 * (y - x1.y);
                    break;
                case 3:
                    attrs_[i].attrValue = triangleInfo.v2.attributes[i] + attrs_[i].d3 * (y - x2.y);
                    break;
                default:
                    break;
                }
            }
            if (x >= 0 && x < target.Width() && y >= 0 && y < target.Height()) {
                target.Set(x, y, Color3{1, 1, 1});
            }
        });
        break;
    case PolygonMode::Point:
        target.Set(x1.x, x1.y, Color3{1, 1, 1});
        target.Set(x2.x, x2.y, Color3{1, 1, 1});
        target.Set(x3.x, x3.y, Color3{1, 1, 1});
        break;
    default:
        break;
    }
}

void Rasterizer::RasterizeTriangleFill(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleFillCallback &&callback) const{
    // Slopes by x : dx / dy
    const float_t dx1 = float_t(x3.x - x1.x) / float_t(x3.y - x1.y);
    const float_t dx2 = x2.y != x1.y ? float_t(x2.x - x1.x) / float_t(x2.y - x1.y) : 0;
    const float_t dx3 = x3.y != x2.y ? float_t(x3.x - x2.x) / float_t(x3.y - x2.y) : 0;

    for (auto y = x1.y; y <= x3.y; ++y) {
        const bool second = y >= x2.y;
        int32_t xl = static_cast<int32_t>(x1.x + dx1 * (y - x1.y));
        int32_t xr = static_cast<int32_t>(second ? x2.x + dx3 * (y - x2.y) : x1.x + dx2 * (y - x1.y));
        bool swapped = false;
        if (xl > xr) {
            std::swap(xl, xr);
            swapped = true;
        }
        for (int32_t x = xl; x <= xr; ++x) {
            callback(x, y, xl, xr, second, swapped);
        }
    }
}

void Rasterizer::RasterizeTriangleLine(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback && callback) const {
    size_t side = 0;
    auto lineCallback = [&side, &callback](int32_t x, int32_t y) {
        callback(x, y, side);
    };
    side = 1;
    LineDDA(x1, x2, lineCallback);
    side = 2;
    LineDDA(x2, x3, lineCallback);
    side = 3;
    LineDDA(x1, x3, lineCallback);
}

void Rasterizer::RasterizeTrianglePoint(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback &&callback) const {
}

void Rasterizer::LineDDA(const Vec2i &x1, const Vec2i &x2, LineCallback &&callback) const {
    // Degenerated line
    if (x1.x == x2.x && x1.y == x2.y) {
        return;
    }

    // Vertical
    if (x1.x == x2.x) {
        for (int32_t y = x1.y; y != x2.y; y += x2.y > x1.y ? 1 : -1) {
            callback(x1.x, y);
        }
        return;
    }

    // Horizontal
    if (x1.y == x2.y) {
        for (int32_t x = x1.x; x != x2.x; x += x2.x > x1.x ? 1 : -1) {
            callback(x, x1.y);
        }
        return;
    }

    uint32_t xlen = std::abs(x2.x - x1.x);
    uint32_t ylen = std::abs(x2.y - x1.y);

    // 45 degrees
    if (xlen == ylen) {
        for (int32_t x = x1.x, y = x1.y; 
             x != x2.x && y != x2.y; 
             x += x2.x > x1.x ? 1 : -1, y += x2.y > x1.y ? 1 : -1) {
            callback(x, y);
        }
        return;
    }

    uint32_t N = ylen;
    if (xlen > ylen) {
        N = xlen;
    }
    float_t dx = float_t(x2.x - x1.x) / N;
    float_t dy = float_t(x2.y - x1.y) / N;
    for (uint32_t i = 0; i < N + 1; ++i) {
        callback(int32_t(std::round(x1.x + dx * i)), int32_t(std::round(x1.y + dy * i)));
    }

}

void Rasterizer::LineBresenham(const Vec2i & x1, const Vec2i & x2, LineCallback && callback) const {}
