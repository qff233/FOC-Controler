#include <cmath>

#include "angle_sensor.h"

AngleSensor::AngleSensor(int32_t max_raw_angle)
        : m_half_max_raw_angle(max_raw_angle / 2) {}

void AngleSensor::update() {
    m_last_angle = m_current_angle;
    m_current_angle = (int32_t) this->update_raw();

    auto delta_angle = m_current_angle - m_last_angle;
    if (std::abs(delta_angle) > m_half_max_raw_angle) {
        if (delta_angle > 0)
            m_lap_count--;
        else
            m_lap_count++;
    }
}

AngleResult AngleSensor::get_angle() {
    return {
            m_lap_count,
            (uint32_t) m_current_angle
    };
}
