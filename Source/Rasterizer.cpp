#include "Rasterizer.h"

#include <stdexcept>
#include <cassert>
#include <algorithm>

#include "Logger.h"
#include "Algorithms.h"

Rasterizer::Rasterizer() {
    std::memset(attributeInfos, 0, RASTERIZER_MAX_ATTRIBUTES * sizeof(AttributeInterpolationInfo));
}

void Rasterizer::DrawTriangle(DrawTriangleInfo &triangleInfo, Texture &target, Texture *depthTarget, PhongFragmentShader &shader) {
    assert(triangleInfo.numAttributes <= RASTERIZER_MAX_ATTRIBUTES);

    if (triangleInfo.numAttributes > 0) {
        assert(triangleInfo.v1.attributes && triangleInfo.v2.attributes && triangleInfo.v3.attributes);
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
    
    // Side heights of triangle
    const float_t sideHeight1 = float_t(x3.y - x1.y);
    const float_t sideHeight2 = float_t(x2.y - x1.y);
    const float_t sideHeight3 = float_t(x3.y - x2.y);

    // Compute attribute slopes
    for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
        attributeInfos[i].d1 = float_t(triangleInfo.v3.attributes[i] - triangleInfo.v1.attributes[i]) / sideHeight1;
        attributeInfos[i].d2 = sideHeight2 > 0 ? float_t(triangleInfo.v2.attributes[i] - triangleInfo.v1.attributes[i]) / sideHeight2 : 0;
        attributeInfos[i].d3 = sideHeight3 > 0 ? float_t(triangleInfo.v3.attributes[i] - triangleInfo.v2.attributes[i]) / sideHeight3 : 0;
    }

    switch (triangleInfo.polygonMode) {
    case PolygonMode::Fill: {
        // Just to make sure y is not equal to first pixel y coordinate
        int32_t ycur = x1.y - 1;
        RasterizeTriangleFill(x1, x2, x3,
            [&](int32_t x, int32_t y, int32_t xl, int32_t xr, bool secondHalf, bool swapped) {
            if (!(x >= int32_t(0) && x < int32_t(target.Width()) &&
                y >= int32_t(0) && y < int32_t(target.Height()))) {
                return;
            }

            if (ycur != y) {
                ycur = y;
                // Changed to the next row so it is needed to calc new interpolated values on both sides of that row
                // and along the row too
                for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                    attributeInfos[i].left  = triangleInfo.v1.attributes[i] + attributeInfos[i].d1 * (y - x1.y);
                    attributeInfos[i].right = secondHalf ? triangleInfo.v2.attributes[i] + attributeInfos[i].d3 * (y - x2.y)
                                                         : triangleInfo.v1.attributes[i] + attributeInfos[i].d2 * (y - x1.y);
                    if (swapped) {
                        std::swap(attributeInfos[i].left, attributeInfos[i].right);
                    }
                }
                // dattrHor will contain interpolation coeffs along the horizontal line
                for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                    attributeInfos[i].dhor = xr - xl > 0 ? (attributeInfos[i].right - attributeInfos[i].left) / float_t(xr - xl) : 0;
                }
            }

            for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                attributeInfos[i].attributeValue = attributeInfos[i].left + attributeInfos[i].dhor * float_t(x - xl);
            }

            // Depth test
            bool depthTestPassed = true;
            if (depthTarget) {
                float_t newDepth = attributeInfos[0].attributeValue;
                assert(newDepth >= -1 && newDepth <= 1);
                float_t currentDepth = 1.0f;
                depthTarget->Get(x, y, 0, &currentDepth);
                if (newDepth > currentDepth) {
                    depthTestPassed = false;
                } else {
                    depthTarget->Set(x, y, 0, &newDepth);
                }
            }

            if (depthTestPassed) {
                // Copy attributes
                float_t attributeValues[RASTERIZER_MAX_ATTRIBUTES];
                for (size_t i = 0, j = depthTarget ? 1 : 0; i < triangleInfo.numAttributes; ++i, ++j) {
                    attributeValues[i] = attributeInfos[j].attributeValue;
                }

                Vec3f fragColor{ shader.Call(attributeValues) };
                target.Set(x, y, fragColor);
            }

        });
        break;
    }
    case PolygonMode::Line:
        RasterizeTriangleLine(x1, x2, x3, [&](int32_t x, int32_t y, size_t side) {
            for (size_t i = 0; i < triangleInfo.numAttributes; ++i) {
                switch (side) {
                case 1:
                    attributeInfos[i].attributeValue = triangleInfo.v1.attributes[i] + attributeInfos[i].d1 * (y - x1.y);
                    break;
                case 2:
                    attributeInfos[i].attributeValue = triangleInfo.v1.attributes[i] + attributeInfos[i].d2 * (y - x1.y);
                    break;
                case 3:
                    attributeInfos[i].attributeValue = triangleInfo.v2.attributes[i] + attributeInfos[i].d3 * (y - x2.y);
                    break;
                default:
                    break;
                }
            }
            if (x >= int32_t(0) && x < int32_t(target.Width()) && 
                y >= int32_t(0) && y < int32_t(target.Height())) {
                target.Set(x, y, Vec3f{1, 1, 1});
            }
        });
        break;
    case PolygonMode::Point:
        target.Set(x1.x, x1.y, Vec3f{1, 1, 1});
        target.Set(x2.x, x2.y, Vec3f{1, 1, 1});
        target.Set(x3.x, x3.y, Vec3f{1, 1, 1});
        break;
    default:
        break;
    }
}
