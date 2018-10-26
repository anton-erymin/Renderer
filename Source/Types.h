#pragma once

#include <cstdint>

#ifdef USE_DOUBLE
using float_t = double;
#else
using float_t = float;
#endif

template <typename T>
struct Vec2 {
	union {
		struct { T x, y; };
		struct { T u, v; };
	};
};

using Vec2i = Vec2<int32_t>;
using Vec2f = Vec2<float_t>;

#include "Vec3.h"
#include "Vec4.h"

using Vec3f = Vec3<float_t>;
using Vec4f = Vec4<float_t>;

#include "Mat3.h"
#include "Mat4.h"

using Mat3f = Mat3<float_t>;
using Mat4f = Mat4<float_t>;