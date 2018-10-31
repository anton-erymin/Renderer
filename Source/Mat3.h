#pragma once

#include <ostream>

#include "Math.h"
#include "Vec3.h"

template <typename T>
struct Mat3 {
	union {
        struct { T a11, a12, a13, a21, a22, a23, a31, a32, a33; };
		T v[9];
        T vv[3][3];
	};

	Mat3() = default;
    Mat3(T s) {}
    Mat3(const T *v) {}
    //Mat3(const Vec4<T> &v) : Mat3(&v.x) {}

    //const Mat3 &operator=(const Vec4<T> &v);

    void Clear() {}
    void Identity() {}  
    void Set(T s) {}
    void Set(const T *v) {}

    void Add(const Mat3 &other) {}
    Mat3 AddR(const Mat3 &other) const {}
    void Sub(const Mat3 &other) {}
    Mat3 SubR(const Mat3 &other) const {}
    void Scale(T s) {}
    Mat3 ScaleR(T s) const {}
    void AddScaled(const Mat3 &other, T s) {}
    Mat3 AddScaledR(const Mat3 &other, T s) const {}

    void Transpose() {}
    void Invert() {}

    Vec3<T> MultTransposed(const Vec3<T> &vec) {}
    void MultLeft(const Mat3 &other) {}

    void Rotate(float angle, const Vec3<T> &axis) {}


    Mat3 operator~() const {}
};



//template <typename T>
//std::ostream &operator<<(std::ostream &os, const Mat3<T> &obj) {
//	return os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
//}
