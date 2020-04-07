#pragma once

#include <math.h>
#include <cmath>
#include "waypointmanager/Inc/math.hpp"

#define sqr(x) ((x * x))

#ifndef INF
    #pragma message "Definition of INF"
    #define INF (1.0 / 0.0)
#endif //ifndef INF

#ifndef NULL
    #define NULL 0x00
#endif //ifndef NULL

#ifndef PI
    #define PI 3.14159
#endif //ifndef PI

// template <typename T> int sgn(T val);
#define sgn(x) ((x < 0.0)? -1 : 1)
// inline int sgn(float a)
// {
//     return ((0 < a) - (a < 0));
// }