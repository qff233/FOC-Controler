#ifndef FOC_FOC_H
#define FOC_FOC_H

#include <cmath>
#include <cstdint>

#include "math/fast_sincos.h"

struct Uvw {
    float u;
    float v;
    float w;
};

struct Dq {
    float d;
    float q;
};

struct SinCos {
    float sinx;
    float cosx;
};

struct Pwm {
    uint32_t u;
    uint32_t v;
    uint32_t w;
};

SinCos sincos(float x) {
    SinCos result{};
    fast_sin_cos(x, &result.sinx, &result.cosx);
    return result;
}

Pwm svpwm(const Dq &dq, const SinCos &sincos);
Dq park(const Uvw &uvw, const SinCos &sincos);

#endif //FOC_FOC_H
