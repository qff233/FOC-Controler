#include "foc.h"
#include "config.h"

#define SQRT3 1.732050807568877f
#define SQRT3_BY_2 (SQRT3 / 2.0f)
#define SQRT3_BY_3 (SQRT3 / 3.0f)

Pwm svpwm(const Dq &dq, const SinCos &sincos) {
    auto [d, q] = dq;
    auto [sin, cos] = sincos;

    auto alpha = cos * d - sin * q;
    auto beta = sin * d + cos * q;

    int32_t half_pwm_period = PWM_MAX / 2;
    int32_t a = (int32_t) alpha * half_pwm_period;
    int32_t b = (int32_t) (-0.5f * alpha + SQRT3_BY_2 * beta) * half_pwm_period;
    int32_t c = (int32_t) (-0.5f * alpha - SQRT3_BY_2 * beta) * half_pwm_period;

    int32_t vmax = std::max(std::max(a, b), c);
    int32_t vmin = std::min(std::min(a, b), c);
    uint32_t vcom = (vmax + vmin) / 2 + half_pwm_period;
    return {
            vcom - a, vcom - b, vcom - c
    };
}

Dq park(const Uvw &uvw, const SinCos &sincos) {
    auto alpha = uvw.u;
    auto beta = SQRT3_BY_3 * (uvw.u + 2.0f * uvw.v);

    auto [sin, cos] = sincos;
    auto d = cos * alpha + sin * beta;
    auto q = -sin * alpha + cos * beta;
    return {d, q};
}
