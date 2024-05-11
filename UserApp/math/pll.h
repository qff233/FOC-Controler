#ifndef FOC_PLL_H
#define FOC_PLL_H

#include <cmath>
#include "utils.h"

class Pll {
public:
    Pll(float kp, float t, float limit);

    float operator()(float value);
    float get_last_output() const;
private:
    float m_kp;
    float m_t;
    float m_limit;

    float m_last_value = 0;
    float m_last_diff = 0.0f;

    float m_last_output = 0.0f;
};


#endif //FOC_PLL_H
