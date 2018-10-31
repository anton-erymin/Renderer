#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#define DEG_TO_RAD(x)  (x * M_PI / 180)
#define RAD_TO_DEG(x)  (x * 180 / M_PI)

#define RAND_RANGE(a, b) ((b - a)* float_t(rand()) / RAND_MAX + a)

#include "Vec3.h"
#include "Vec4.h"
#include "Mat3.h"
#include "Mat4.h"

template <typename T>
struct Vec2 {
    union {
        struct { T x, y; };
        struct { T u, v; };
    };
};

using Vec2i = Vec2<int32_t>;
using Vec2f = Vec2<float_t>;
using Vec3f = Vec3<float_t>;
using Vec4f = Vec4<float_t>;
using Mat3f = Mat3<float_t>;
using Mat4f = Mat4<float_t>;

template <typename T>
inline T Clamp(T value, T min, T max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

template <typename T>
T Dot(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return lhs.Dot(rhs);
}

template <typename T>
T Dot(const Vec4<T> &lhs, const Vec4<T> &rhs) {
    return lhs.Dot(rhs);
}

template <typename T>
Vec3<T> Normalize(const Vec3<T> &v) {
    Vec3<T> result = v;
    result.Normalize();
    return result;
}

template <typename T>
Vec4<T> Normalize(const Vec4<T> &v) {
    Vec4<T> result = v;
    result.Normalize();
    return result;
}

template <typename T>
Vec3<T> Clamp(const Vec3<T> &v, T min, T max) {
    return { Clamp(v.x, min, max), Clamp(v.y, min, max), Clamp(v.z, min, max) };
}

template <typename T>
Vec4<T> Clamp(const Vec4<T> &v, T min, T max) {
    return { Clamp(v.x), Clamp(v.y), Clamp(v.z), Clamp(v.w) };
}

template <typename T>
Vec3<T> Pow(const Vec3<T> &v, T exp) {
    return { std::powf(v.x, exp), std::powf(v.y, exp), std::powf(v.z, exp) };
}

template <typename T>
Vec4<T> Pow(const Vec4<T> &v, T exp) {
    return { std::powf(v.x, exp), std::powf(v.y, exp), std::powf(v.z, exp), std::powf(v.w, exp) };
}

template <typename T>
inline Vec3<T> Reflect(const Vec3<T> &v, const Vec3<T> &n) {
    return -2 * Dot(v, n) * n + v;
}

inline bool IsFaceCCW(const Vec4f & v1, const Vec4f & v2, const Vec4f & v3) {
    return ((v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y)) > 0;
}