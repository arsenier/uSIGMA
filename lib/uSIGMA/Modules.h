#pragma once

#include "Motor/EncDriver.h"
#include "Motor/MotorDriver.h"

EncoderConnectionParams encconpar = {
    .ENCA_PIN = 2,
    .ENCB_PIN = 3,
    .ENC_PORT = &PIND,
    .ENC_MASK = 0b00001100,
    .ENC_SHIFT = 2,
    .ENC_DIR = 1,
    .ENC_PPR = 48,
    .i = 47,
};

EncDriver encoder(&encconpar);

void init_encoder()
{
    // https://thewanderingengineer.com/2014/08/11/arduino-pin-change-interrupts/
    // PCICR |= 0b00000001; // turn on port b
    PCICR |= 0b00000100; // turn on port d change interrupts

    // PCMSK0 |= 0b00000011; // turn on pins PB0 and PB1 (D8 and D9)
    PCMSK2 |= 0b00001100; // turn on pins PD2 & PD3 (D2 and D3)
    interrupts();
}

ISR(PCINT2_vect) // Port D, PCINT16 - PCINT23
{
    encoder.interruptHandler();
}

const volatile float &enc_phi = encoder.phi;

////////////////////

MotorConnectionParams mconnp = {
    .IN = 10,
    .PWM = 11,
    .MOTOR_DIR = 1,
};

volatile float target_voltage, supply_voltage;

MotorDriver motor(target_voltage, supply_voltage, &mconnp);
