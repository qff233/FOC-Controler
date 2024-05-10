#ifndef FOC_PID_H
#define FOC_PID_H


class Pid {
public:
    Pid(float p, float i, float d, float interval_time, float forward = 0.0f, float ramp = 0.0f,
        float integral_limit = 0.0f, float output_limit = 0.0f);

    float operator()(float error);
public:
    float p;
    float i;
    float d;
    float forward;
    float ramp;
    float integral_limit;
    float output_limit;
private:
    float m_interval_time;
    float m_last_error = 0.0f;
    float m_last_output = 0.0f;
    float m_last_integral = 0.0f;
};


#endif //FOC_PID_H
