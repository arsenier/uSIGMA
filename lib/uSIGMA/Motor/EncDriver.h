#pragma once

#include <Arduino.h>
#include "Types.h"

struct EncoderConnectionParams
{
    int ENCA_PIN;
    //
    int ENCB_PIN;
    //
    volatile uint8_t *ENC_PORT;
    uint8_t ENC_MASK;
    uint8_t ENC_SHIFT;
    int8_t ENC_DIR;
    //
    uint16_t ENC_PPR;
    uint16_t i;
};

class EncDriver : public Tickable, public EncoderConnectionParams
{
private:
    int8_t ett[4][4] = {0};
    volatile uint8_t enc_zn1 = 0;
    volatile int16_t counter = 0;

    float tick_to_rad;
    float q_phi;

public:
    const float &phi = q_phi;

    EncDriver(EncoderConnectionParams *encConnParms)
        : EncoderConnectionParams(*encConnParms)
    {
        ett[0b00][0b10] = ENC_DIR;
        ett[0b10][0b11] = ENC_DIR;
        ett[0b11][0b01] = ENC_DIR;
        ett[0b01][0b00] = ENC_DIR;

        ett[0b00][0b01] = -ENC_DIR;
        ett[0b01][0b11] = -ENC_DIR;
        ett[0b11][0b10] = -ENC_DIR;
        ett[0b10][0b00] = -ENC_DIR;

        tick_to_rad = 2.0 * M_PI / (ENC_PPR * i);

        pinMode(ENCA_PIN, INPUT);
        pinMode(ENCB_PIN, INPUT);
    }

    void tick() override
    {
        noInterrupts();
        const int16_t counter_inc = counter;
        counter = 0;
        interrupts();

        q_phi += counter_inc * tick_to_rad;
    }

    void interruptHandler();
};

void EncDriver::interruptHandler()
{
    const uint8_t enc = ((*ENC_PORT) & ENC_MASK) >> ENC_SHIFT;
    counter += ett[enc_zn1][enc];
    enc_zn1 = enc;
}
