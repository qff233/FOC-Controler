#include "main.h"

#include "motor.h"

static Motor *motor;

int Main() {

    auto open_velocity_motor = OpenVelocityMotor{{7}, {0.0f, 0.2f}, 0.785, 20000};
    motor = &open_velocity_motor;
    while (true) {
    }
}

// 20KHz
void adc_interrupt(ADC_HandleTypeDef *hadc) {
    static uint32_t tim1_count = 0;
    tim1_count++;
    if (tim1_count / 20 == 1) { // 1KHz
        motor->position_tick();
    }
    if (tim1_count / 10 == 1) {  // 2KHz
        motor->velocity_tick();
    }

    motor->current_tick(Uvw());
}
