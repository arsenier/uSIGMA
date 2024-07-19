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
        : MotorConnectionParams(*mconnp), target_voltage(target_voltage_), supply_voltage(supply_voltage_)
    {
        pinMode(IN, INPUT);
        pinMode(PWM, INPUT);
    }

    void tick() override
    {
        cli();
        const int16_t pwm = 255.0 * constrain(target_voltage / supply_voltage, -1.0, 1.0) * MOTOR_DIR;
        sei();

        const int16_t sat_pwm = constrain(pwm, -255, 255);

        if (sat_pwm >= 0)
        {
            digitalWrite(IN, LOW);
            analogWrite(PWM, sat_pwm);
        }
        else
        {
            digitalWrite(IN, HIGH);
            analogWrite(PWM, 255 + sat_pwm);
        }
    }
};
