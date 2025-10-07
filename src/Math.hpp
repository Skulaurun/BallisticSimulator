#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

template<typename T>
struct Vector3 {
    T x;
    T y;
    T z;

    inline T magnitude() {
        return std::sqrt(x * x + y * y + z * z);
    }
    inline Vector3<T> normalize() {
        const T length = magnitude();
        return Vector3<T>(x / length, y / length, z / length);
    }

    Vector3<T> operator*(const T scalar) const { return Vector3<T>(x * scalar, y * scalar, z * scalar); }
    Vector3<T> operator/(const T scalar) const { return Vector3<T>(x / scalar, y / scalar, z / scalar); }
    Vector3<T> operator+(const T scalar) const { return Vector3<T>(x + scalar, y + scalar, z + scalar); }
    Vector3<T> operator-(const T scalar) const { return Vector3<T>(x - scalar, y - scalar, z - scalar); }

    Vector3<T> operator+(const Vector3<T>& other) const { return Vector3<T>(x + other.x, y + other.y, z + other.z); }
    Vector3<T> operator-(const Vector3<T>& other) const { return Vector3<T>(x - other.x, y - other.y, z - other.z); }
    Vector3<T>& operator+=(const Vector3<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vector3<T>& operator-=(const Vector3<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

    Vector3<T> operator-() const { return Vector3<T>{ -x, -y, -z }; }
};

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

// standard-like functions
inline float radf(float deg) {
    return deg * M_PI / 180.0f;
}
inline double rad(double deg) {
    return deg * M_PI / 180.0;
}

inline float degf(float rad) {
    return rad * 180.0f / M_PI;
}

inline double deg(double rad) {
    return rad * 180.0 / M_PI;
}

}
