#include <cstdio>

#include "motor.h"
#include "foc.h"

Motor::Motor(MotorParams params)
        : m_params(params) {}

void Motor::set_velocity(float velocity) {
    printf("Current control mode cannot set velocity!");
}

void Motor::set_position(const LapAngle &position) {
    printf("Current control mode cannot set position!");
}

/// Open velocity
OpenVelocityMotor::OpenVelocityMotor(MotorParams params, Dq udq, float velocity, uint32_t frequency)
        : Motor(params), m_udq(udq) {
    m_rad_per_tick = velocity / (float) frequency;
}

Pwm OpenVelocityMotor::current_tick(const Uvw &i) {
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
VelocityWithSensorMotor::VelocityWithSensorMotor(MotorParams params, AngleSensor &sensor, Pid current_d_pid,
                                                 Pid current_q_pid,
                                                 Pid velocity_pid, Pll velocity_pll, float target_velocity)
        : Motor(params),
          m_sensor(sensor),
          m_current_d_pid(current_d_pid),
          m_current_q_pid(current_q_pid),
          m_velocity_pid(velocity_pid),
          m_velocity_pll(velocity_pll),
          m_target_velocity(target_velocity) {}

void VelocityWithSensorMotor::set_velocity(float velocity) {
    m_target_velocity = velocity;
}

Pwm VelocityWithSensorMotor::current_tick(const Uvw &i) {
    m_sensor.update();
    auto angle = m_sensor.get_angle();
    m_velocity_pll(angle.angle);

    SinCos sin_cos = sincos(angle.angle * (float) m_params.m_pole_num);
    auto [d, q] = park(i, sin_cos);
    float d_error = 0.0f - d;
    float q_error = m_velocity_pid.get_last_output() - q;
    float u_d = m_current_d_pid(d_error);
    float u_q = m_current_q_pid(q_error);
    return svpwm({u_d, u_q}, sin_cos);
}

void VelocityWithSensorMotor::velocity_tick() {
    auto error = m_target_velocity - m_velocity_pll.get_last_output();
    m_velocity_pid(error);
}
////////////////////////

/// position with sensor
PositionWithSensorMotor::PositionWithSensorMotor(MotorParams params, AngleSensor &sensor, Pid current_d_pid,
                                                 Pid current_q_pid,
                                                 Pid velocity_pid, Pid position_pid, Pll velocity_pll,
                                                 LapAngle target_position)
        : Motor(params),
          m_sensor(sensor),
          m_current_d_pid(current_d_pid),
          m_current_q_pid(current_d_pid),
          m_velocity_pid(velocity_pid),
          m_position_pid(position_pid),
          m_velocity_pll(velocity_pll),
          m_target_position(target_position) {}

void PositionWithSensorMotor::set_position(const LapAngle &position) {
    m_target_position = position;
}

Pwm PositionWithSensorMotor::current_tick(const Uvw &i) {
    m_sensor.update();
    auto angle = m_sensor.get_angle();
    m_velocity_pll(angle.angle);

    auto sin_cos = sincos(angle.angle * (float) m_params.m_pole_num);
    auto [d, q] = park(i, sin_cos);
    float d_error = 0.0f - d;
    float q_error = m_velocity_pid.get_last_output() - q;
    float u_d = m_current_d_pid(d_error);
    float u_q = m_current_q_pid(q_error);
    return svpwm({u_d, u_q}, sin_cos);
}

void PositionWithSensorMotor::velocity_tick() {
    auto error = m_position_pid.get_last_output() - m_velocity_pll.get_last_output();
    m_velocity_pid(error);
}

void PositionWithSensorMotor::position_tick() {
    auto [current_lap_count, current_angle] = m_sensor.get_angle();
    auto [target_lap_count, target_angle] = m_target_position;
    float error = (float) (target_lap_count - current_lap_count) * _2PI + (target_angle - current_angle);
    m_position_pid(error);
}
////////////////////////
