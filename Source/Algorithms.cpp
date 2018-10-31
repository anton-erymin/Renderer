#include "Algorithms.h"

#include <cassert>

/* Rasterization algorithms. */

// Side 1 from v1 - v3
// Side 2 from v1 - v2
// Side 3 from v2 - v3

void RasterizeTriangleFill(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleFillCallback &&callback) {
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


void RasterizeTriangleLine(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback && callback) {
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

void RasterizeTrianglePoint(const Vec2i &x1, const Vec2i &x2, const Vec2i &x3, TriangleLineCallback &&callback) {}

void LineDDA(const Vec2i &x1, const Vec2i &x2, LineCallback &&callback) {
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
    if (N > 100000) {
        return;
    }
    float_t dx = float_t(x2.x - x1.x) / N;
    float_t dy = float_t(x2.y - x1.y) / N;
    for (uint32_t i = 0; i < N + 1; ++i) {
        callback(int32_t(std::round(x1.x + dx * i)), int32_t(std::round(x1.y + dy * i)));
    }

}

void LineBresenham(const Vec2i & x1, const Vec2i & x2, LineCallback && callback) {}

void GammaCorrection(Texture &target, float_t gamma) {
    // Gamma correction
    for (size_t y = 0; y < target.Height(); ++y) {
        for (size_t x = 0; x < target.Width(); ++x) {
            Vec3f color{};
            target.Get(x, y, &color);
            color = Pow(color, gamma);
            target.Set(x, y, &color);
        }
    }
}

void ReinhardTonemapping(Texture &target) {
    for (size_t y = 0; y < target.Height(); ++y) {
        for (size_t x = 0; x < target.Width(); ++x) {
            Vec3f color{};
            target.Get(x, y, &color);
            color = color / (color + Vec3f{ 1.0f });
            target.Set(x, y, &color);
        }
    }
}

void Blur(Texture &target, size_t iterations) {
    assert(iterations >= 1);

    Texture temp{"BlurTemp", target.Width(), target.Height(), target.GetNumComponents(), target.GetBytesPerComponent(), false};

    Texture *A = &target;
    Texture *B = &temp;

    for (size_t i = 0; i < iterations; i++) {
        for (size_t y = 1; y < target.Height() - 1; ++y) {
            for (size_t x = 1; x < target.Width() - 1; ++x) {
                Vec3f result{ 0.0f };
                for (size_t j = 0; j < 3; ++j) {
                    for (size_t i = 0; i < 3; ++i) {
                        Vec3f sample{ 0.0f };
                        A->Get(x + i - 1, y + j - 1, &sample);
                        result += sample;
                    }
                }
                result /= 9.0f;
                B->Set(x, y, &result);
            }
        }
        std::swap(A, B);
    }
    
    if (&target != A) {
        temp.MoveTo(target);
    }
}

void GaussianBlurInDirection(const Texture &src, Texture &dst, const std::vector<float_t> &kernel, bool vertical) {
    assert(src.IsCompatibleWith(dst));

    size_t offset = kernel.size() - 1;
    for (size_t y = offset; y < src.Height() - offset; ++y) {
        for (size_t x = offset; x < src.Width() - offset; ++x) {

            Vec3f result{ 0.0f };

            for (size_t i = 0; i < kernel.size(); ++i) {

                Vec3f sample{ 0.0f };

                if (!vertical) {
                    src.Get(x - i, y, &sample);
                    result += sample * kernel[i];
                    src.Get(x + i, y, &sample);
                    result += sample * kernel[i];
                } else {
                    src.Get(x, y - i, &sample);
                    result += sample * kernel[i];
                    src.Get(x, y - i, &sample);
                    result += sample * kernel[i];
                }
            }

            dst.Set(x, y, &result);
        }
    }
}

void GaussianBlur(Texture &target, const std::vector<float_t> &kernel, size_t iterations) {
    assert(iterations >= 1);
    assert(kernel.size() > 1);

    Texture temp{ "GaussianBlurTemp", target.Width(), target.Height(), target.GetNumComponents(), target.GetBytesPerComponent(), false };

    for (size_t i = 0; i < iterations; ++i) {
        GaussianBlurInDirection(target, temp, kernel, false);
        GaussianBlurInDirection(temp, target, kernel, true);
    }
}
