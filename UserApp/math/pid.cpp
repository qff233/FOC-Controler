//
// Created by 34763 on 2024/5/10.
//

#include <cmath>
#include "pid.h"
#include "utils.h"

Pid::Pid(float p, float i, float d, float interval_time, float forward, float ramp,
         float integral_limit, float output_limit)
        : p(p), i(i), d(d), forward(forward) {
    m_interval_time = std::abs(interval_time);
    this->ramp = std::abs(ramp);
    this->integral_limit = std::abs(interval_time);
    this->output_limit = std::abs(output_limit);
}

float Pid::operator()(float error) {
    auto dt = m_interval_time;

    float p_term = p * error;
    float i_term = m_last_integral + i * dt * (error);
    i_term = CONSTRAIN(i_term, integral_limit, -integral_limit);
    float d_term = d * (error - m_last_error) / dt;

    float output = p_term + i_term + d_term;
    if (forward != 0.0f) {
        output += forward * error;
    }
    if (ramp > 0.0f) {
        float max_delta_output = ramp * dt;
        output = CONSTRAIN(output, m_last_output + max_delta_output, m_last_output - max_delta_output);
    }
    output = CONSTRAIN(output, output_limit, -output_limit);

    m_last_integral = i_term;
    m_last_output = output;
    m_last_error = error;

    return output;
}

float Pid::get_last_output() const {
    return m_last_output;
}
