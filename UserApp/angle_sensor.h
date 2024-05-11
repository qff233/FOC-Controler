#ifndef FOC_ANGLE_SENSOR_H
#define FOC_ANGLE_SENSOR_H


#include <cstdint>

enum class Direction {
    CW = 1,
    CCW = -1
};

struct LapAngle {
    int32_t lap_count;
    float angle;
};

class AngleSensor {
public:
    virtual ~AngleSensor() = default;

    void update();

    LapAngle get_angle();
protected:
    virtual float update_raw() = 0;
private:
    int32_t m_lap_count = 0;
    float m_last_angle = 0;
    float m_current_angle = 0;
};


#endif //FOC_ANGLE_SENSOR_H
