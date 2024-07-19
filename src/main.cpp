#include <Arduino.h>

#include "Channels.h"
#include "Modules.h"
#include "tau.h"
#include "tiper.h"

#define SLIDER0 A0
#define SLIDER1 A1
#define SLIDER2 A2

#define Ts_us 100000
#define Ts_s (Ts_us * 1.0 / 1000000)

void setup()
{
  Serial.begin(9600);
  pinMode(SLIDER1, INPUT);
  pinMode(SLIDER2, INPUT);
  init_encoder();
  timer_init();
}

int slider0;
int slider1;
int slider2;

#define LOG(name, var) name + String(var) +

volatile float phi0 = 0;

void loop()
{
  //////// TIMER ////////
  static uint32_t timer0 = 0;
  while (micros() - timer0 < Ts_us)
    ;
  timer0 = micros();

  //////// SENSE ////////
  slider1 = analogRead(SLIDER1) - 512;

  phi0 = slider1 * 10.0 / 512;

  //////// PLAN ////////

  //////// ACT ////////

  Serial.println(
      LOG(" phi0: ", phi0) //
      // LOG(" w0: ", w0)           //
      // LOG(" ws: ", ws)           //
      LOG(" phi: ", encoder.phi) //
      //
      "" //
  );
}
