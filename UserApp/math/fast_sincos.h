//
// Created by czt on 2023/1/30.
//
// refer to
// https://github.com/samhocevar/lolremez/wiki/Tutorial-4-of-5%3A-fixing-lower-order-parameters
// https://github.com/chenzt2020/foc_learning.git

#ifndef FOC_FAST_SINCOS_H
#define FOC_FAST_SINCOS_H

#define F_PI 3.14159265358979323846f

// lolremez --float --degree 5 --range "1e-50:pi*pi"
// "(sin(sqrt(x))-sqrt(x))/(x*sqrt(x))" "1/(x*sqrt(x))"
// Estimated max error: 1.455468e-9
inline float f1(float x) {
    float u = 1.3528548e-10f;
    u = u * x + -2.4703144e-08f;
    u = u * x + 2.7532926e-06f;
    u = u * x + -0.00019840381f;
    u = u * x + 0.0083333179f;
    return u * x + -0.16666666f;
}

// lolremez --float --degree 5 --range "1e-50:pi*pi" "(cos(sqrt(x))-1)/x"
// "1/x"
// Estimated max error: 1.1846383e-8
inline float f2(float x) {
    float u = 1.7290616e-09f;
    u = u * x + -2.7093486e-07f;
    u = u * x + 2.4771643e-05f;
    u = u * x + -0.0013887906f;
    u = u * x + 0.041666519f;
    return u * x + -0.49999991f;
}

inline float fast_sin(float x) {
    // si = (int)(x / pi)
    int si = (int) (x * 0.31830988f);
    x = x - (float) si * F_PI;
    if (si & 1) {
        x = x > 0.0f ? x - F_PI : x + F_PI;
    }
    return x + x * x * x * f1(x * x);
}

inline float fast_cos(float x) {
    // si = (int)(x / pi)
    int si = (int) (x * 0.31830988f);
    x = x - (float) si * F_PI;
    if (si & 1) {
        x = x > 0.0f ? x - F_PI : x + F_PI;
    }
    return 1.0f + x * x * f2(x * x);
}

inline void fast_sin_cos(float x, float *sin_x, float *cos_x) {
    // si = (int)(x / pi)
    int si = (int) (x * 0.31830988f);
    x = x - (float) si * F_PI;
    if (si & 1) {
        x = x > 0.0f ? x - F_PI : x + F_PI;
    }
    *sin_x = x + x * x * x * f1(x * x);
    *cos_x = 1.0f + x * x * f2(x * x);
}

#endif //FOC_FAST_SINCOS_H
