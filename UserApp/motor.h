#ifndef FOC_MOTOR_H
#define FOC_MOTOR_H

#include "math/pid.h"
#include "foc.h"
#include "angle_sensor.h"
#include "math/pll.h"

struct MotorParams {
    uint32_t m_pole_num;
    float m_encoder_offset = 0.0f;
    float m_resistance = 0.0f;
    float m_inductance = 0.0f;
};

class Motor {
public:
    explicit Motor(MotorParams params);
    virtual ~Motor() = default;

    virtual void set_velocity(float velocity);
    virtual void set_position(const LapAngle &position);

    virtual Pwm current_tick(const Uvw &i) = 0;

    virtual void velocity_tick() {}

    virtual void position_tick() {}

protected:
    MotorParams m_params;
};

class OpenVelocityMotor : public Motor {
public:
    OpenVelocityMotor(MotorParams params, Dq udq, float velocity, uint32_t frequency);
    Pwm current_tick(const Uvw &i) override;
private:
    Dq m_udq;
    float m_current_position = 0.0f;
    float m_rad_per_tick;
};

class VelocityWithSensorMotor : public Motor {
public:
    VelocityWithSensorMotor(MotorParams params, AngleSensor &sensor, Pid current_d_pid, Pid current_q_pid,
                            Pid velocity_pid, Pll velocity_pll, float target_velocity = 0.0f);

    void set_velocity(float velocity) override;

    Pwm current_tick(const Uvw &i) override;
    void velocity_tick() override;
private:
    AngleSensor &m_sensor;
    Pid m_current_d_pid;
    Pid m_current_q_pid;
    Pid m_velocity_pid;
    Pll m_velocity_pll;

    float m_target_velocity;
};

class PositionWithSensorMotor : public Motor {
public:
    PositionWithSensorMotor(MotorParams params, AngleSensor &sensor, Pid current_d_pid, Pid current_q_pid,
                            Pid velocity_pid, Pid position_pid, Pll velocity_pll, LapAngle target_position);

    void set_position(const LapAngle &velocity) override;

    Pwm current_tick(const Uvw &i) override;
    void velocity_tick() override;
    void position_tick() override;
private:
    AngleSensor &m_sensor;
    Pid m_current_d_pid;
    Pid m_current_q_pid;
    Pid m_velocity_pid;
    Pid m_position_pid;
    Pll m_velocity_pll;

    LapAngle m_target_position;
};

#endif //FOC_MOTOR_H
