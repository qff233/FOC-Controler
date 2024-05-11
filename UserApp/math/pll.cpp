#include "pll.h"

#include "utils.h"

Pll::Pll(float kp, float t, float limit)
        : m_kp(kp), m_t(t) {
    m_limit = std::abs(limit);
}

float Pll::operator()(float value) {
    float diff = value - m_last_value;
    if (diff > _PI)
        diff -= _2PI;
    if (diff < -_PI)
        diff += _2PI;

    m_last_value = value;
    m_last_diff += (diff - m_last_diff) * m_kp;
    m_last_diff = CONSTRAIN(m_last_diff, m_limit, -m_limit);

    m_last_output = m_last_diff / m_t;
    return m_last_output;
}

float Pll::get_last_output() const {
    return m_last_output;
}
