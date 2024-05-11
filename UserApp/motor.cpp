#include <cstdio>

#include "motor.h"
#include "foc.h"

#define _2PI 6.2831853f

void Motor::set_velocity(float velocity) {
    printf("Current control mode cannot set velocity!");
}

void Motor::set_position(float position) {
    printf("Current control mode cannot set position!");
}

/// Open velocity
OpenVelocityMotor::OpenVelocityMotor(Dq udq, float velocity, uint32_t frequency)
        : m_udq(udq) {
    m_rad_per_tick = velocity / (float) frequency;
}

Pwm OpenVelocityMotor::current_tick() {
    m_current_position += m_rad_per_tick;
    if (m_current_position > _2PI)
        m_current_position -= _2PI;
    if (m_current_position < 0)
        m_current_position += _2PI;

    auto sin_cos = sincos(m_current_position);
    return svpwm(m_udq, sin_cos);
}
////////////////////////

/// Velocity with sensor
VelocityWithSensorMotor::VelocityWithSensorMotor(Pid current_pid, Pid velocity_pid, float target_velocity)
        : m_current_pid(current_pid), m_velocity_pid(velocity_pid), m_target_velocity(target_velocity) {}

void VelocityWithSensorMotor::set_velocity(float velocity) {
    Motor::set_velocity(velocity);
}

Pwm VelocityWithSensorMotor::current_tick() {
    return Pwm();
}

void VelocityWithSensorMotor::velocity_tick() {

}
////////////////////////

/// position with sensor
PositionWithSensorMotor::PositionWithSensorMotor(Pid current_pid, Pid velocity_pid, Pid position_pid,
                                                 float target_position)
        : m_current_pid(current_pid), m_velocity_pid(velocity_pid), m_position_pid(position_pid),
          m_target_position(target_position) {}

void PositionWithSensorMotor::set_velocity(float velocity) {
    Motor::set_velocity(velocity);
}

Pwm PositionWithSensorMotor::current_tick() {
    return Pwm();
}

void PositionWithSensorMotor::velocity_tick() {
    Motor::velocity_tick();
}

void PositionWithSensorMotor::position_tick() {
    Motor::position_tick();
}

////////////////////////
