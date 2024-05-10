#ifndef FOC_PLL_H
#define FOC_PLL_H

#include <cmath>
#include "utils.h"

template<typename T>
class Pll {
public:
    Pll(T rand, float kp, float t, float limit);

    T operator()(T value);
private:
    T m_rand;
    float m_kp;
    float m_t;
    float m_limit;

    T m_last_value = 0;
    float m_last_diff = 0.0;
};

template<typename T>
Pll<T>::Pll(T rand, float kp, float t, float limit)
        : m_kp(kp), m_t(t) {
    m_rand = std::abs(rand);
    m_limit = std::abs(limit);
}

template<typename T>
T Pll<T>::operator()(T value) {
    T diff = value - m_last_value;
    if (diff > m_rand / 2)
        diff -= m_rand;
    if (diff < -m_rand / 2)
        diff += m_rand;

    m_last_value = value;
    m_last_diff += (diff - m_last_diff) * m_kp;
    m_last_diff = CONSTRAIN(m_last_diff, m_limit, -m_limit);
    return m_last_diff / m_t;
}

#endif //FOC_PLL_H
