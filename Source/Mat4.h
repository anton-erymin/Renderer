#pragma once

#include <ostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Vec4.h"

#define DEG_TO_RAD(x)  (x * M_PI / 180)
#define RAD_TO_DEG(x)  (x * 180 / M_PI)

#define MAT4_NUM_ELEMENTS   (16)

template <typename T>
struct Mat4 {
	union {
        struct { T a11, a12, a13, a14, 
                   a21, a22, a23, a24, 
                   a31, a32, a33, a34, 
                   a41, a42, a43, a44; };
		T v[16];
        T vv[4][4];
	};

    Mat4();
    Mat4(T s) {}
    Mat4(const T *v) {}

    void Clear();
    void Identity();
    void Set(T s) {}
    void Set(const T *v) {}

    void Add(const Mat4 &other) {}
    Mat4 AddR(const Mat4 &other) const {}
    void Sub(const Mat4 &other) {}
    Mat4 SubR(const Mat4 &other) const {}
    void Scale(T s) {}
    Mat4 ScaleR(T s) const {}
    void AddScaled(const Mat4 &other, T s) {}
    Mat4 AddScaledR(const Mat4 &other, T s) const {}

    void Transpose() {}
    void Invert() {}

    Vec3<T> MultTransposed(const Vec3<T> &vec) {}
    void MultLeft(const Mat4 &other) {}

    void Rotate(float angle, const Vec3<T> &axis) {}


    Mat4 operator~() const {}

    void PerspectiveProjection(T fov, T aspect, T znear, T zfar);
    void LookAt(const Vec3<T> &eye, const Vec3<T> &target, const Vec3<T> &up);
};

template <typename T>
Mat4<T>::Mat4() {
    Identity();
}

//
//
//Mat4<T>::Mat4<T>(float s) {
//    m_data[0][0] = s; m_data[0][1] = s; m_data[0][2] = s;
//    m_data[1][0] = s; m_data[1][1] = s; m_data[1][2] = s;
//    m_data[2][0] = s; m_data[2][1] = s; m_data[2][2] = s;
//}
//
//
//Mat4<T>::Mat4<T>(float **v) {
//    m_data[0][0] = v[0][0]; m_data[0][1] = v[0][1]; m_data[0][2] = v[0][2];
//    m_data[1][0] = v[1][0]; m_data[1][1] = v[1][1]; m_data[1][2] = v[1][2];
//    m_data[2][0] = v[2][0]; m_data[2][1] = v[2][1]; m_data[2][2] = v[2][2];
//}

template <typename T>
void Mat4<T>::Clear() {
    std::memset(v, 0, MAT4_NUM_ELEMENTS * sizeof(T));
}

template <typename T>
void Mat4<T>::Identity() {
    Clear();
    for (size_t i = 0; i < 4; i++) {
        vv[i][i] = T(1);
    }
}

//void Mat4<T>::add(const Mat4<T> &other) {
//    m_data[0][0] += other.m_data[0][0]; m_data[0][1] += other.m_data[0][1]; m_data[0][2] += other.m_data[0][2];
//    m_data[1][0] += other.m_data[1][0]; m_data[1][1] += other.m_data[1][1]; m_data[1][2] += other.m_data[1][2];
//    m_data[2][0] += other.m_data[2][0]; m_data[2][1] += other.m_data[2][1]; m_data[2][2] += other.m_data[2][2];
//}
//
//
//Mat4<T> Mat4<T>::addR(const Mat4<T> &other) {
//    Mat4<T> res;
//
//    res.m_data[0][0] = m_data[0][0] + other.m_data[0][0];
//    res.m_data[0][1] = m_data[0][1] + other.m_data[0][1];
//    res.m_data[0][2] = m_data[0][2] + other.m_data[0][2];
//
//    res.m_data[1][0] = m_data[1][0] + other.m_data[1][0];
//    res.m_data[1][1] = m_data[1][1] + other.m_data[1][1];
//    res.m_data[1][2] = m_data[1][2] + other.m_data[1][2];
//
//    res.m_data[2][0] = m_data[2][0] + other.m_data[2][0];
//    res.m_data[2][1] = m_data[2][1] + other.m_data[2][1];
//    res.m_data[2][2] = m_data[2][2] + other.m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::sub(const Mat4<T> &other) {
//    m_data[0][0] -= other.m_data[0][0]; m_data[0][1] -= other.m_data[0][1]; m_data[0][2] -= other.m_data[0][2];
//    m_data[1][0] -= other.m_data[1][0]; m_data[1][1] -= other.m_data[1][1]; m_data[1][2] -= other.m_data[1][2];
//    m_data[2][0] -= other.m_data[2][0]; m_data[2][1] -= other.m_data[2][1]; m_data[2][2] -= other.m_data[2][2];
//}
//
//
//Mat4<T> Mat4<T>::subR(const Mat4<T> &other) {
//    Mat4<T> res;
//
//    res.m_data[0][0] = m_data[0][0] - other.m_data[0][0];
//    res.m_data[0][1] = m_data[0][1] - other.m_data[0][1];
//    res.m_data[0][2] = m_data[0][2] - other.m_data[0][2];
//
//    res.m_data[1][0] = m_data[1][0] - other.m_data[1][0];
//    res.m_data[1][1] = m_data[1][1] - other.m_data[1][1];
//    res.m_data[1][2] = m_data[1][2] - other.m_data[1][2];
//
//    res.m_data[2][0] = m_data[2][0] - other.m_data[2][0];
//    res.m_data[2][1] = m_data[2][1] - other.m_data[2][1];
//    res.m_data[2][2] = m_data[2][2] - other.m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::scale(float s) {
//    m_data[0][0] *= s; m_data[0][1] *= s; m_data[0][2] *= s;
//    m_data[1][0] *= s; m_data[1][1] *= s; m_data[1][2] *= s;
//    m_data[2][0] *= s; m_data[2][1] *= s; m_data[2][2] *= s;
//}
//
//
//Mat4<T> Mat4<T>::scaleR(float s) {
//    Mat4<T> res;
//
//    res.m_data[0][0] = s * m_data[0][0];
//    res.m_data[0][1] = s * m_data[0][1];
//    res.m_data[0][2] = s * m_data[0][2];
//
//    res.m_data[1][0] = s * m_data[1][0];
//    res.m_data[1][1] = s * m_data[1][1];
//    res.m_data[1][2] = s * m_data[1][2];
//
//    res.m_data[2][0] = s * m_data[2][0];
//    res.m_data[2][1] = s * m_data[2][1];
//    res.m_data[2][2] = s * m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::addScaled(const Mat4<T> &other, float s) {
//    m_data[0][0] += s * other.m_data[0][0]; m_data[0][1] += s * other.m_data[0][1]; m_data[0][2] += s * other.m_data[0][2];
//    m_data[1][0] += s * other.m_data[1][0]; m_data[1][1] += s * other.m_data[1][1]; m_data[1][2] += s * other.m_data[1][2];
//    m_data[2][0] += s * other.m_data[2][0]; m_data[2][1] += s * other.m_data[2][1]; m_data[2][2] += s * other.m_data[2][2];
//}
//
//
//Mat4<T> Mat4<T>::addScaledR(const Mat4<T> &other, float s) {
//    Mat4<T> res;
//
//    res.m_data[0][0] = m_data[0][0] + s * other.m_data[0][0];
//    res.m_data[0][1] = m_data[0][1] + s * other.m_data[0][1];
//    res.m_data[0][2] = m_data[0][2] + s * other.m_data[0][2];
//
//    res.m_data[1][0] = m_data[1][0] + s * other.m_data[1][0];
//    res.m_data[1][1] = m_data[1][1] + s * other.m_data[1][1];
//    res.m_data[1][2] = m_data[1][2] + s * other.m_data[1][2];
//
//    res.m_data[2][0] = m_data[2][0] + s * other.m_data[2][0];
//    res.m_data[2][1] = m_data[2][1] + s * other.m_data[2][1];
//    res.m_data[2][2] = m_data[2][2] + s * other.m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::transpose() {
//    float temp;
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            temp = m_data[i][j];
//            m_data[i][j] = m_data[j][i];
//            m_data[j][i] = temp;
//        }
//    }
//}
//
//
//lpVec3 Mat4<T>::multTransposed(const lpVec3 &vec) {
//    lpVec3 res;
//
//    res.m_x = m_data[0][0] * vec.m_x + m_data[1][0] * vec.m_y + m_data[2][0] * vec.m_z;
//    res.m_y = m_data[0][1] * vec.m_x + m_data[1][1] * vec.m_y + m_data[2][1] * vec.m_z;
//    res.m_z = m_data[0][2] * vec.m_x + m_data[1][2] * vec.m_y + m_data[2][2] * vec.m_z;
//
//    return res;
//}
//
//
//void Mat4<T>::multLeft(const Mat4<T> &other) {
//    float m00 = other.m_data[0][0] * m_data[0][0] + other.m_data[0][1] * m_data[1][0] + other.m_data[0][2] * m_data[2][0];
//    float m01 = other.m_data[0][0] * m_data[0][1] + other.m_data[0][1] * m_data[1][1] + other.m_data[0][2] * m_data[2][1];
//    float m02 = other.m_data[0][0] * m_data[0][2] + other.m_data[0][1] * m_data[1][2] + other.m_data[0][2] * m_data[2][2];
//
//    float m10 = other.m_data[1][0] * m_data[0][0] + other.m_data[1][1] * m_data[1][0] + other.m_data[1][2] * m_data[2][0];
//    float m11 = other.m_data[1][0] * m_data[0][1] + other.m_data[1][1] * m_data[1][1] + other.m_data[1][2] * m_data[2][1];
//    float m12 = other.m_data[1][0] * m_data[0][2] + other.m_data[1][1] * m_data[1][2] + other.m_data[1][2] * m_data[2][2];
//
//    float m20 = other.m_data[2][0] * m_data[0][0] + other.m_data[2][1] * m_data[1][0] + other.m_data[2][2] * m_data[2][0];
//    float m21 = other.m_data[2][0] * m_data[0][1] + other.m_data[2][1] * m_data[1][1] + other.m_data[2][2] * m_data[2][1];
//    float m22 = other.m_data[2][0] * m_data[0][2] + other.m_data[2][1] * m_data[1][2] + other.m_data[2][2] * m_data[2][2];
//
//    m_data[0][0] = m00; m_data[0][1] = m01; m_data[0][2] = m02;
//    m_data[1][0] = m10; m_data[1][1] = m11; m_data[1][2] = m12;
//    m_data[2][0] = m20; m_data[2][1] = m21; m_data[2][2] = m22;
//}
//
//
//void Mat4<T>::rotate(float angle, const lpVec3 &axis) {
//    float c = cosf(angle);
//    float s = sinf(angle);
//    float c1 = 1.0f - c;
//    float u1 = axis.m_x;
//    float u2 = axis.m_y;
//    float u3 = axis.m_z;
//    float u12 = u1 * u2;
//    float u13 = u1 * u3;
//    float u23 = u2 * u3;
//
//    Mat4<T> rm;
//    rm.m_data[0][0] = c1 * u1 * u1 + c;	 rm.m_data[0][1] = c1 * u12 - s * u3; rm.m_data[0][2] = c1 * u13 + s * u2;
//    rm.m_data[1][0] = c1 * u12 + s * u3; rm.m_data[1][1] = c1 * u2 * u2 + c;  rm.m_data[1][2] = c1 * u23 - s * u1;
//    rm.m_data[2][0] = c1 * u13 - s * u2; rm.m_data[2][1] = c1 * u23 + s * u1; rm.m_data[2][2] = c1 * u3 * u3 + c;
//
//    multLeft(rm);
//}
//
//
//Mat4<T> Mat4<T>::operator+(const Mat4<T> &other) {
//    Mat4<T> res;
//
//    res.m_data[0][0] = m_data[0][0] + other.m_data[0][0];
//    res.m_data[0][1] = m_data[0][1] + other.m_data[0][1];
//    res.m_data[0][2] = m_data[0][2] + other.m_data[0][2];
//
//    res.m_data[1][0] = m_data[1][0] + other.m_data[1][0];
//    res.m_data[1][1] = m_data[1][1] + other.m_data[1][1];
//    res.m_data[1][2] = m_data[1][2] + other.m_data[1][2];
//
//    res.m_data[2][0] = m_data[2][0] + other.m_data[2][0];
//    res.m_data[2][1] = m_data[2][1] + other.m_data[2][1];
//    res.m_data[2][2] = m_data[2][2] + other.m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::operator +=(const Mat4<T> &other) {
//    m_data[0][0] += other.m_data[0][0]; m_data[0][1] += other.m_data[0][1]; m_data[0][2] += other.m_data[0][2];
//    m_data[1][0] += other.m_data[1][0]; m_data[1][1] += other.m_data[1][1]; m_data[1][2] += other.m_data[1][2];
//    m_data[2][0] += other.m_data[2][0]; m_data[2][1] += other.m_data[2][1]; m_data[2][2] += other.m_data[2][2];
//}
//
//
//Mat4<T> Mat4<T>::operator-(const Mat4<T> &other) {
//    Mat4<T> res;
//
//    res.m_data[0][0] = m_data[0][0] - other.m_data[0][0];
//    res.m_data[0][1] = m_data[0][1] - other.m_data[0][1];
//    res.m_data[0][2] = m_data[0][2] - other.m_data[0][2];
//
//    res.m_data[1][0] = m_data[1][0] - other.m_data[1][0];
//    res.m_data[1][1] = m_data[1][1] - other.m_data[1][1];
//    res.m_data[1][2] = m_data[1][2] - other.m_data[1][2];
//
//    res.m_data[2][0] = m_data[2][0] - other.m_data[2][0];
//    res.m_data[2][1] = m_data[2][1] - other.m_data[2][1];
//    res.m_data[2][2] = m_data[2][2] - other.m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::operator-=(const Mat4<T> &other) {
//    m_data[0][0] -= other.m_data[0][0]; m_data[0][1] -= other.m_data[0][1]; m_data[0][2] -= other.m_data[0][2];
//    m_data[1][0] -= other.m_data[1][0]; m_data[1][1] -= other.m_data[1][1]; m_data[1][2] -= other.m_data[1][2];
//    m_data[2][0] -= other.m_data[2][0]; m_data[2][1] -= other.m_data[2][1]; m_data[2][2] -= other.m_data[2][2];
//}
//
//
//lpVec3 Mat4<T>::operator*(const lpVec3 &vec) {
//    lpVec3 res;
//
//    res.m_x = m_data[0][0] * vec.m_x + m_data[0][1] * vec.m_y + m_data[0][2] * vec.m_z;
//    res.m_y = m_data[1][0] * vec.m_x + m_data[1][1] * vec.m_y + m_data[1][2] * vec.m_z;
//    res.m_z = m_data[2][0] * vec.m_x + m_data[2][1] * vec.m_y + m_data[2][2] * vec.m_z;
//
//    return res;
//}
//
//
//Mat4<T> Mat4<T>::operator*(const Mat4<T> &other) {
//    Mat4<T> res;
//
//    res.m_data[0][0] = m_data[0][0] * other.m_data[0][0] + m_data[0][1] * other.m_data[1][0] + m_data[0][2] * other.m_data[2][0];
//    res.m_data[0][1] = m_data[0][0] * other.m_data[0][1] + m_data[0][1] * other.m_data[1][1] + m_data[0][2] * other.m_data[2][1];
//    res.m_data[0][2] = m_data[0][0] * other.m_data[0][2] + m_data[0][1] * other.m_data[1][2] + m_data[0][2] * other.m_data[2][2];
//
//    res.m_data[1][0] = m_data[1][0] * other.m_data[0][0] + m_data[1][1] * other.m_data[1][0] + m_data[1][2] * other.m_data[2][0];
//    res.m_data[1][1] = m_data[1][0] * other.m_data[0][1] + m_data[1][1] * other.m_data[1][1] + m_data[1][2] * other.m_data[2][1];
//    res.m_data[1][2] = m_data[1][0] * other.m_data[0][2] + m_data[1][1] * other.m_data[1][2] + m_data[1][2] * other.m_data[2][2];
//
//    res.m_data[2][0] = m_data[2][0] * other.m_data[0][0] + m_data[2][1] * other.m_data[1][0] + m_data[2][2] * other.m_data[2][0];
//    res.m_data[2][1] = m_data[2][0] * other.m_data[0][1] + m_data[2][1] * other.m_data[1][1] + m_data[2][2] * other.m_data[2][1];
//    res.m_data[2][2] = m_data[2][0] * other.m_data[0][2] + m_data[2][1] * other.m_data[1][2] + m_data[2][2] * other.m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::operator *=(const Mat4<T> &other) {
//    float m00 = m_data[0][0] * other.m_data[0][0] + m_data[0][1] * other.m_data[1][0] + m_data[0][2] * other.m_data[2][0];
//    float m01 = m_data[0][0] * other.m_data[0][1] + m_data[0][1] * other.m_data[1][1] + m_data[0][2] * other.m_data[2][1];
//    float m02 = m_data[0][0] * other.m_data[0][2] + m_data[0][1] * other.m_data[1][2] + m_data[0][2] * other.m_data[2][2];
//
//    float m10 = m_data[1][0] * other.m_data[0][0] + m_data[1][1] * other.m_data[1][0] + m_data[1][2] * other.m_data[2][0];
//    float m11 = m_data[1][0] * other.m_data[0][1] + m_data[1][1] * other.m_data[1][1] + m_data[1][2] * other.m_data[2][1];
//    float m12 = m_data[1][0] * other.m_data[0][2] + m_data[1][1] * other.m_data[1][2] + m_data[1][2] * other.m_data[2][2];
//
//    float m20 = m_data[2][0] * other.m_data[0][0] + m_data[2][1] * other.m_data[1][0] + m_data[2][2] * other.m_data[2][0];
//    float m21 = m_data[2][0] * other.m_data[0][1] + m_data[2][1] * other.m_data[1][1] + m_data[2][2] * other.m_data[2][1];
//    float m22 = m_data[2][0] * other.m_data[0][2] + m_data[2][1] * other.m_data[1][2] + m_data[2][2] * other.m_data[2][2];
//
//    m_data[0][0] = m00; m_data[0][1] = m01; m_data[0][2] = m02;
//    m_data[1][0] = m10; m_data[1][1] = m11; m_data[1][2] = m12;
//    m_data[2][0] = m20; m_data[2][1] = m21; m_data[2][2] = m22;
//}
//
//
//
//Mat4<T> Mat4<T>::operator~() {
//    Mat4<T> res;
//
//    res.m_data[0][0] = m_data[0][0]; res.m_data[0][1] = m_data[1][0]; res.m_data[0][2] = m_data[2][0];
//    res.m_data[1][0] = m_data[0][1]; res.m_data[1][1] = m_data[1][1]; res.m_data[1][2] = m_data[2][1];
//    res.m_data[2][0] = m_data[0][2]; res.m_data[2][1] = m_data[1][2]; res.m_data[2][2] = m_data[2][2];
//
//    return res;
//}
//
//
//void Mat4<T>::invert() {
//    float det = m_data[0][0] * m_data[1][1] * m_data[2][2] - m_data[0][0] * m_data[1][2] * m_data[2][1];
//    det -= m_data[0][1] * m_data[1][0] * m_data[2][2] + m_data[0][1] * m_data[1][2] * m_data[2][0];
//    det += m_data[0][2] * m_data[1][0] * m_data[2][1] - m_data[0][2] * m_data[1][1] * m_data[2][0];
//
//    if (det != 0.0f) {
//        det = 1.0f / det;
//
//        float m00 = m_data[1][1] * m_data[2][2] - m_data[2][1] * m_data[1][2];
//        float m01 = -(m_data[0][1] * m_data[2][2] - m_data[2][1] * m_data[0][2]);
//        float m02 = m_data[0][1] * m_data[1][2] - m_data[1][1] * m_data[0][2];
//
//        float m10 = -(m_data[1][0] * m_data[2][2] - m_data[2][0] * m_data[1][2]);
//        float m11 = m_data[0][0] * m_data[2][2] - m_data[2][0] * m_data[0][2];
//        float m12 = -(m_data[0][0] * m_data[1][2] - m_data[1][0] * m_data[0][2]);
//
//        float m20 = m_data[1][0] * m_data[2][1] - m_data[2][0] * m_data[1][1];
//        float m21 = -(m_data[0][0] * m_data[2][1] - m_data[2][0] * m_data[0][1]);
//        float m22 = m_data[0][0] * m_data[1][1] - m_data[1][0] * m_data[0][1];
//
//        m_data[0][0] = det * m00; m_data[0][1] = det * m01; m_data[0][2] = det * m02;
//        m_data[1][0] = det * m10; m_data[1][1] = det * m11; m_data[1][2] = det * m12;
//        m_data[2][0] = det * m20; m_data[2][1] = det * m21; m_data[2][2] = det * m22;
//    }
//}

template<typename T>
void Mat4<T>::PerspectiveProjection(T fov, T aspect, T n, T f) {
    Clear();

    T r = T(std::tan(0.5 * DEG_TO_RAD(fov)) * n);
    T l = -r;
    T t = r / aspect;
    T b = -t;

    a11 = 2 * n / (r - l);
    a13 = (r + l) / (r - l);
    a22 = 2 * n / (t - b);
    a23 = (t + b) / (t - b);
    a33 = -(f + n) / (f - n);
    a34 = -2 * f * n / (f - n);
    a43 = -1;
    a44 = 1;
}

template<typename T>
void Mat4<T>::LookAt(const Vec3<T> &eye, const Vec3<T> &target, const Vec3<T> &up) {
    Identity();
    Vec3<T> Z = eye - target;
    Z.Normalize();
    Vec3<T> X = up % Z;
    X.Normalize();
    Vec3<T> Y = Z % X;
    Y.Normalize();

    a11 = X.x;
    a12 = X.y;
    a13 = X.z;
    a21 = Y.x;
    a22 = Y.y;
    a23 = Y.z;
    a31 = Z.x;
    a32 = Z.y;
    a33 = Z.z;
    a14 = -X.Dot(eye);
    a24 = -Y.Dot(eye);
    a34 = -Z.Dot(eye);
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Mat4<T> &obj) {
	return os << std::endl 
              << "(" 
              << obj.a11 << "  " << obj.a12 << "  " << obj.a13 << "  " << obj.a14 << std::endl << " "
              << obj.a21 << "  " << obj.a22 << "  " << obj.a23 << "  " << obj.a24 << std::endl << " "
              << obj.a31 << "  " << obj.a32 << "  " << obj.a33 << "  " << obj.a34 << std::endl << " "
              << obj.a41 << "  " << obj.a42 << "  " << obj.a43 << "  " << obj.a44 
              << ")";
}

template<typename T>
Mat4<T> operator*(const Mat4<T> &lhs, const Mat4<T> &rhs) {
    Mat4<T> res;
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            res.vv[i][j] = lhs.vv[i][0] * rhs.vv[0][j] 
                         + lhs.vv[i][1] * rhs.vv[1][j] 
                         + lhs.vv[i][2] * rhs.vv[2][j] 
                         + lhs.vv[i][3] * rhs.vv[3][j];
        }
    } 
    return res;
}

template<typename T>
Vec4<T> operator*(const Mat4<T> &lhs, const Vec4<T> &rhs) {
    Vec4<T> res;
    for (size_t i = 0; i < 4; ++i) {
        res.v[i] = lhs.vv[i][0] * rhs.x 
                 + lhs.vv[i][1] * rhs.y 
                 + lhs.vv[i][2] * rhs.z 
                 + lhs.vv[i][3] * rhs.w;
    }
    return res;
}