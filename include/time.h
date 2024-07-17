#pragma once
#include <Arduino.h>

#define OCR0A_inc (500 / 4)  /*!< [1/250 ms] */
#define OCR0B_inc (1000 / 4) /*!< [1/250 ms] */

/**
 * OCR0A -  2kHz
 * OCR0B -  1kHz
 */

static volatile uint8_t OCR0A_val;
static volatile uint8_t OCR0B_val;

void timer0_comp_init()
{
    //! v TIMER0 CMPB Interrupt
    TIMSK0 |= (1 << OCIE0B) | (0 << OCIE0A); // Enables OCIE0B
    OCR0A_val = OCR0A_inc;
    OCR0A = OCR0A_val;
    OCR0B_val = OCR0B_inc;
    OCR0B = OCR0B_val;
}

void typerA_inc()
{
    OCR0A_val += OCR0A_inc;
    OCR0A = OCR0A_val;
}

void typerB_inc()
{
    OCR0B_val += OCR0B_inc;
    OCR0B = OCR0B_val;
    // OCR0B = 128;
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{
    typerA_inc();
}

int8_t counter = 0;
int32_t counter2 = 0;
uint32_t time0 = 0;
uint32_t time1 = 0;
uint32_t dtime = 0;

ISR(TIMER0_COMPB_vect, ISR_NOBLOCK)
{
    typerB_inc();
    counter++;
    counter %= 100;

    // if (counter == 0)
    // {
    time1 = micros();
    dtime = time1 - time0;
    time0 = time1;
    // counter2++;
    Serial.print(dtime);
    Serial.print(" ");
    Serial.print(OCR0B);
    Serial.print(" ");
    Serial.println(counter);
    // }
}
