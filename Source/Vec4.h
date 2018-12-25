#pragma once

#include <ostream>

#include "Math.h"

template <typename T>
struct Vec3;

template <typename T>
struct Vec4 {
	union {
		struct { T x, y, z, w; };
        struct { T r, g, b, a; };
		T v[4];
	};

	Vec4() = default;
	Vec4(T xv, T yv, T zv, T wv) : x(xv), y(yv), z(zv), w(wv) {}
    Vec4(T xv, T yv, T zv) : Vec4(xv, yv, zv, T(1)) {}
	Vec4(T s) : x(s), y(s), z(s), w(s) {}
	Vec4(const T *values) : x(values[0]), y(values[1]), z(values[2]), w(values[3]) {}
    Vec4(const Vec3<T> &vec3, T w = T(1)) : Vec4(vec3.x, vec3.y, vec3.z, w) {}

    const Vec4 &operator=(const Vec3<T> &vec3);

	void Clear() { x = T(0); y = T(0); z = T(0); w = T(0); }
	void Set(T xv, T yv, T zv, T wv) { x = xv; y = yv; z = zv; w = wv; }
	void Set(T s) { x = s; y = s; z = s; w = s; }
	void Set(const T *values) { x = values[0]; y = values[1]; z = values[2]; w = values[3]; }

	void Add(const Vec4 &other);
	Vec4 AddR(const Vec4 &other) const;
	void Sub(const Vec4 &other);
	Vec4 SubR(const Vec4 &other) const;
	void Scale(T s);
	Vec4 ScaleR(T s) const;		
	void AddScaled(const Vec4 &other, T s);
	Vec4 AddScaledR(const Vec4 &other, T s) const;

	T	 Dot(const Vec4 &other) const;
	//Vec4 Cross(const Vec4 &other) const;

	T    Length() const;
	T    LengthSq() const;
	void Normalize();

	bool IsZero() const { return x == T(0) && y == T(0) && z == T(0) && w == T(0); }
	bool IsUnitLength() const { return LengthSq() == T(1); }
};

template <typename T>
const Vec4<T> &Vec4<T>::operator=(const Vec3<T> &vec3) {
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    w = T(1);
    return *this;
}

template <typename T>
void Vec4<T>::Add(const Vec4 &other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
}

template <typename T>
Vec4<T> Vec4<T>::AddR(const Vec4 &other) const {
	return Vec4<T>{x + other.x, y + other.y, z + other.z, w + other.w};
}

template <typename T>
void Vec4<T>::Sub(const Vec4 &other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
    w -= other.w;
}

template <typename T>
Vec4<T> Vec4<T>::SubR(const Vec4 &other) const {
	return Vec4<T>{x - other.x, y - other.y, z - other.z, w - other.w};
}

template <typename T>
void Vec4<T>::Scale(T s) {
	x *= s;
	y *= s;
	z *= s;
    w *= s;
}

template <typename T>
Vec4<T> Vec4<T>::ScaleR(T s) const {
	return Vec4<T>{s * x, s * y, s * z, s * w};
}

template <typename T>
void Vec4<T>::AddScaled(const Vec4& other, T s) {
	x += s * other.x;
	y += s * other.y;
	z += s * other.z;
    w += s * other.w;
}

template <typename T>
Vec4<T> Vec4<T>::AddScaledR(const Vec4& other, T s) const {
	return Vec4<T>{x + s * other.x, y + s * other.y, z + s * other.z, w + s * other.w};
}

template <typename T>
T Vec4<T>::Dot(const Vec4& other) const {
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

//template <typename T>
//Vec4<T> Vec4<T>::Cross(const Vec4& other) const {
//	return Vec4<T>{y * other.z - z * other.y,
//				   z * other.x - x * other.z,
//				   x * other.y - y * other.x};
//}

template <typename T>
T Vec4<T>::Length() const {
	return std::sqrt(x * x + y * y + z * z + w * w);
}

template <typename T>
T Vec4<T>::LengthSq() const {
	return x * x + y * y + z * z + w * w;
}

template <typename T>
void Vec4<T>::Normalize() {
	const T norm = (T)std::sqrt(x * x + y * y + z * z + w * w);
	if (norm > 0) {
		x /= norm;
		y /= norm;
		z /= norm;
        w /= norm;
	}
}

template <typename T>
Vec4<T> operator+(const Vec4<T> &lhs, const Vec4<T> &rhs) {
	return lhs.AddR(rhs);
}

template <typename T>
void operator+=(Vec4<T> &lhs, const Vec4<T> &rhs) { 
	lhs.Add(rhs); 
}

template <typename T>
Vec4<T> operator+(const Vec4<T> &lhs, const T rhs) {
    return {lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs};
}

template <typename T>
Vec4<T> operator-(const Vec4<T> &lhs, const Vec4<T> &rhs) {
	return lhs.SubR(rhs);
}

template <typename T>
void operator-=(Vec4<T> &lhs, const Vec4<T> &rhs) {
    lhs.Sub(rhs);
}

template <typename T>
Vec4<T> operator-(const Vec4<T> &lhs, const T rhs) {
    return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs};
}

template <typename T>
Vec4<T> operator*(T s, const Vec4<T> &rhs) {
	return rhs.ScaleR(s);
}

template <typename T>
Vec4<T> operator*(const Vec4<T> &lhs, T s) {
	return lhs.ScaleR(s);
}

template <typename T>
Vec4<T> operator*(const Vec4<T> &lhs, const Vec4<T> &rhs) {
    return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
}

template <typename T>
void operator*=(Vec4<T> &lhs, T s) {
    return lhs.Scale(s);
}

template <typename T>
void operator/=(const Vec4<T> &lhs, T s) {
    lhs.Scale(T(1) / s);
}

//template <typename T>
//Vec4<T> operator%(const Vec4<T> &lhs, const Vec4<T> &rhs) {
//	return lhs.Cross(rhs);
//}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vec4<T> &obj) {
	return os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ")";
}
