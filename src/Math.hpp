#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

// standard-like functions
inline float radf(float deg) {
    return deg * static_cast<float>(M_PI) / 180.0f;
}
inline double rad(double deg) {
    return deg * M_PI / 180.0;
}

inline float degf(float rad) {
    return rad * 180.0f / static_cast<float>(M_PI);
}

inline double deg(double rad) {
    return rad * 180.0 / M_PI;
}

}
