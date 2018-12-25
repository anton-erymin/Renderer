#pragma once

#include <ostream>

#include "Math.h"

template <typename T>
struct Vec4;

template <typename T>
struct Vec3 {
	union {
		struct { T x, y, z; };
        struct { T r, g, b; };
		T v[3];
	};

	Vec3() = default;
	Vec3(T xv, T yv, T zv) : x(xv), y(yv), z(zv) {}
	Vec3(T s) : x(s), y(s), z(s) {}
	Vec3(const T *values) : x(values[0]), y(values[1]), z(values[2]) {}
    Vec3(const Vec4<T> &vec4) : Vec3(&vec4.x) {}

    const Vec3 &operator=(const Vec4<T> &vec4);

	void Clear() { x = T(0); y = T(0); z = T(0); }
	void Set(T xv, T yv, T zv) { x = xv; y = yv; z = zv; }
	void Set(T s) { x = s; y = s; z = s; }
	void Set(const T *values) { x = values[0]; y = values[1]; z = values[2]; }

	void Add(const Vec3 &other);
	Vec3 AddR(const Vec3 &other) const;
	void Sub(const Vec3 &other);
	Vec3 SubR(const Vec3 &other) const;
	void Scale(T s);
	Vec3 ScaleR(T s) const;
	void AddScaled(const Vec3 &other, T s);
	Vec3 AddScaledR(const Vec3 &other, T s) const;

	T    Dot(const Vec3 &other) const;
	Vec3 Cross(const Vec3 &other) const;

	T    Length() const;
	T    LengthSq() const;
	void Normalize();

	bool IsZero() const { return x == T(0) && y == T(0) && z == T(0); }
	bool IsUnitLength() const { return LengthSq() == T(1); }
};

template <typename T>
const Vec3<T> &Vec3<T>::operator=(const Vec4<T> &vec4) {
    x = vec4.x;
    y = vec4.y;
    z = vec4.z;
    return *this;
}

template <typename T>
void Vec3<T>::Add(const Vec3 &other) {
	x += other.x;
	y += other.y;
	z += other.z;
}

template <typename T>
Vec3<T> Vec3<T>::AddR(const Vec3 &other) const {
	return Vec3<T>{x + other.x, y + other.y, z + other.z};
}

template <typename T>
void Vec3<T>::Sub(const Vec3 &other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

template <typename T>
Vec3<T> Vec3<T>::SubR(const Vec3 &other) const {
	return Vec3<T>{x - other.x, y - other.y, z - other.z};
}

template <typename T>
void Vec3<T>::Scale(T s) {
	x *= s;
	y *= s;
	z *= s;
}

template <typename T>
Vec3<T> Vec3<T>::ScaleR(T s) const {
	return Vec3<T>{s * x, s * y, s * z};
}

template <typename T>
void Vec3<T>::AddScaled(const Vec3& other, T s) {
	x += s * other.x;
	y += s * other.y;
	z += s * other.z;
}

template <typename T>
Vec3<T> Vec3<T>::AddScaledR(const Vec3& other, T s) const {
	return Vec3<T>{x + s * other.x, y + s * other.y, z + s * other.z};
}

template <typename T>
T Vec3<T>::Dot(const Vec3& other) const {
	return x * other.x + y * other.y + z * other.z;
}

template <typename T>
Vec3<T> Vec3<T>::Cross(const Vec3& other) const {
	return Vec3<T>{y * other.z - z * other.y,
				   z * other.x - x * other.z,
				   x * other.y - y * other.x};
}

template <typename T>
T Vec3<T>::Length() const {
	return std::sqrt(x * x + y * y + z * z);
}

template <typename T>
T Vec3<T>::LengthSq() const {
	return x * x + y * y + z * z;
}

template <typename T>
void Vec3<T>::Normalize() {
	const T norm = (T)std::sqrt(x * x + y * y + z * z);
	if (norm > 0) {
		x /= norm;
		y /= norm;
		z /= norm;
	}
}

template <typename T>
Vec3<T> operator+(const Vec3<T> &lhs, const Vec3<T> &rhs) {
	return lhs.AddR(rhs);
}

template <typename T>
Vec3<T> operator+(const Vec3<T> &lhs, const T rhs) {
    return {lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};
}

template <typename T>
void operator+=(Vec3<T> &lhs, const Vec3<T> &rhs) { 
	lhs.Add(rhs); 
}

template <typename T>
Vec3<T> operator-(const Vec3<T> &lhs, const Vec3<T> &rhs) {
	return lhs.SubR(rhs);
}

template <typename T>
void operator-=(const Vec3<T> &lhs, const Vec3<T> &rhs) {
	lhs.Sub(rhs);
}

template <typename T>
Vec3<T> operator-(const Vec3<T> &lhs, const T rhs) {
    return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs};
}

template <typename T>
Vec3<T> operator*(T s, const Vec3<T> &rhs) {
	return rhs.ScaleR(s);
}

template <typename T>
Vec3<T> operator*(const Vec3<T> &lhs, T s) {
	return lhs.ScaleR(s);
}

template <typename T>
Vec3<T> operator*(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}

template <typename T>
Vec3<T> operator/(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

template <typename T>
void operator*=(Vec3<T> &lhs, T s) {
	lhs.Scale(s);
}

template <typename T>
void operator/=(Vec3<T> &lhs, T s) {
    lhs.Scale(T(1) / s);
}

template <typename T>
Vec3<T> operator%(const Vec3<T> &lhs, const Vec3<T> &rhs) {
	return lhs.Cross(rhs);
}

template <typename T>
void operator%=(Vec3<T> &lhs, const Vec3<T> &rhs) { 
	lhs = lhs.Cross(rhs); 
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vec3<T> &obj) {
	return os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
}
