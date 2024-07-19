#pragma once

#include <Arduino.h>

#include "Channels.h"
#include "Modules.h"
#include "tau.h"

#define Ts_2khz_us 500
#define Ts_2khz_s (Ts_2khz_us * 1.0 / 1000000)

/*!v 2kHz */
#define OCR0A_inc (500 / 4) /*!< [1/250 ms] */

static volatile uint8_t OCR0A_val;

void timer_init()
{
    TIMSK0 |= (1 << OCIE0A); // Enables OCIE0A
    OCR0A_val = OCR0A_inc;
    OCR0A = OCR0A_val;
}

void tiperA_inc()
{
    OCR0A_val += OCR0A_inc;
    OCR0A = OCR0A_val;
}

volatile float w0, ws;

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{
    //////// TIMER ////////
    tiperA_inc();

    //////// SENSE ////////
    encoder.tick();
    //////// PLAN ////////

    supply_voltage = 12;

    const float eps = phi0 - encoder.phi;

    const float usupply = 12;
    const float Tf = 20 * Ts_2khz_s;

    const float T1 = 0.1, Tu = Ts_2khz_s + Tf; // [s]
    const float Ke = 12 / 7;                   // [V/rpm]

    static Integrator w_estimator(Ts_2khz_s);
    const float e = encoder.phi - w_estimator.out;
    ws = e / Tf;
    w_estimator.tick(ws);

    const float K1 = 1 / Ke;
    const float K = T1 / (0.8 * Tu * K1);
    const float T = T1;
    const float Tu2 = Tu * 2;

    const float Kp = K;
    const float Ki = K / T;
    const float Kphi = 1 / (2 * Tu2);

    w0 = eps * Kphi;

    static Integrator i_reg(Ts_2khz_s);
    const float err = w0 - ws;
    const float p = err * Kp;
    const float i = i_reg.out * Ki;
    const float u = p + i;

    if (u == constrain(u, -usupply, usupply) || (err * i) < 0)
        i_reg.tick(err);

    target_voltage = u;

    //////// ACT ////////
    motor.tick();
}
