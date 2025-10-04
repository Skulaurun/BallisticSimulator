#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

template<typename T>
struct Point3 {
    T x;
    T y;
    T z;
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
