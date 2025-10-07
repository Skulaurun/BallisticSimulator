#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

template<typename T>
struct Point3 {
    T x;
    T y;
    T z;

    inline T magnitude() {
        return std::sqrt(x * x + y * y + z * z);
    }
    inline Point3<T> normalize() {
        const T length = magnitude();
        return Point3<T>(x / length, y / length, z / length);
    }

    Point3<T> operator*(const T scalar) const { return Point3<T>(x * scalar, y * scalar, z * scalar); }
    Point3<T> operator/(const T scalar) const { return Point3<T>(x / scalar, y / scalar, z / scalar); }
    Point3<T> operator+(const T scalar) const { return Point3<T>(x + scalar, y + scalar, z + scalar); }
    Point3<T> operator-(const T scalar) const { return Point3<T>(x - scalar, y - scalar, z - scalar); }

    Point3<T> operator+(const Point3<T>& other) const { return Point3<T>(x + other.x, y + other.y, z + other.z); }
    Point3<T> operator-(const Point3<T>& other) const { return Point3<T>(x - other.x, y - other.y, z - other.z); }
    Point3<T>& operator+=(const Point3<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Point3<T>& operator-=(const Point3<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

    Point3<T> operator-() const { return Point3<T>{ -x, -y, -z }; }
};

using Point3f = Point3<float>;
using Point3d = Point3<double>;

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
