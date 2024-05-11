#ifndef FOC_MOTOR_H
#define FOC_MOTOR_H

#include "math/pid.h"
#include "foc.h"

class Motor {
public:
    virtual ~Motor() = default;

    virtual void set_velocity(float velocity);
    virtual void set_position(float position);

    virtual Pwm current_tick() = 0;

    virtual void velocity_tick() {}

    virtual void position_tick() {}

protected:
};

class OpenVelocityMotor : public Motor {
public:
    OpenVelocityMotor(Dq udq, float velocity, uint32_t frequency);
    Pwm current_tick() override;
private:
    Dq m_udq;
    float m_current_position = 0.0f;
    float m_rad_per_tick;
};

class VelocityWithSensorMotor : public Motor {
public:
    VelocityWithSensorMotor(Pid current_pid, Pid velocity_pid, float target_velocity);

    void set_velocity(float velocity) override;

    Pwm current_tick() override;
    void velocity_tick() override;
private:
    Pid m_current_pid;
    Pid m_velocity_pid;

    float m_target_velocity;
};

class PositionWithSensorMotor : public Motor {
public:
    PositionWithSensorMotor(Pid current_pid, Pid velocity_pid, Pid position_pid, float target_pisition);

    void set_velocity(float velocity) override;

    Pwm current_tick() override;
    void velocity_tick() override;
    void position_tick() override;
private:
    Pid m_current_pid;
    Pid m_velocity_pid;
    Pid m_position_pid;

    float m_target_position;
};

#endif //FOC_MOTOR_H
