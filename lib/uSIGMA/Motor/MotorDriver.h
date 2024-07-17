#pragma once

#include <Arduino.h>
#include "Types.h"

struct MotorConnectionParams
{
    int8_t IN;
    int8_t PWM;

    int8_t MOTOR_DIR;
};

class MotorDriver : public Tickable, public MotorConnectionParams
{
private:
    const volatile float &target_voltage;
    const volatile float &supply_voltage;

public:
    MotorDriver(const volatile float &target_voltage_, const volatile float &supply_voltage_, MotorConnectionParams *mconnp)
        : target_voltage(target_voltage_), supply_voltage(supply_voltage_), MotorConnectionParams(*mconnp)
    {
        pinMode(IN, INPUT);
        pinMode(PWM, INPUT);
    }

    void tick() override
    {
        cli();
        const int16_t pwm = 255.0 * target_voltage / supply_voltage * MOTOR_DIR;
        sei();

        if (pwm >= 0)
        {
            digitalWrite(IN, LOW);
            analogWrite(PWM, pwm);
        }
        else
        {
            digitalWrite(IN, HIGH);
            analogWrite(PWM, 255 + pwm);
        }
    }
};
