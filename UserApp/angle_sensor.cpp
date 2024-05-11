#include <cmath>

#include "angle_sensor.h"
#include "utils.h"

void AngleSensor::update() {
    m_last_angle = m_current_angle;
    m_current_angle = this->update_raw();

    auto delta_angle = m_current_angle - m_last_angle;
    if (std::abs(delta_angle) > _PI) {
        if (delta_angle > 0)
            m_lap_count--;
        else
            m_lap_count++;
    }
}

LapAngle AngleSensor::get_angle() {
    return {
            m_lap_count,
            m_current_angle
    };
}
