#ifndef FOC_ANGLE_SENSOR_H
#define FOC_ANGLE_SENSOR_H


#include <cstdint>

enum class Direction {
    CW = 1,
    CCW = -1
};

struct AngleResult {
    int32_t lap_count;
    uint32_t angle;
};

class AngleSensor {
public:
    explicit AngleSensor(int32_t max_raw_angle);
    virtual ~AngleSensor() = default;

    virtual uint32_t update_raw() = 0;
    void update();

    AngleResult get_angle();
protected:
    int32_t m_lap_count = 0;
    int32_t m_last_angle = 0;
    int32_t m_current_angle = 0;
private:
    int32_t m_half_max_raw_angle;
};


#endif //FOC_ANGLE_SENSOR_H
