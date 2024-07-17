#include <Arduino.h>

#include "Channels.h"
#include "Modules.h"

#define SLIDER0 A0
#define SLIDER1 A1
#define SLIDER2 A2

#define Ts_us 100000
#define Ts_s (Ts_us * 1.0 / 1 000 000)

void setup()
{
  Serial.begin(9600);
  pinMode(SLIDER1, INPUT);
  pinMode(SLIDER2, INPUT);
  init_encoder();
}

int slider0;
int slider1;
int slider2;

#define LOG(name, var) name + String(var) +

void loop()
{
  //////// TIMER ////////
  static uint32_t timer0 = 0;
  while (micros() - timer0 < Ts_us)
    ;
  timer0 = micros();

  //////// SENSE ////////
  encoder.tick();

  //////// PLAN ////////

  //////// ACT ////////
  Serial.println(
      LOG(" phi: ", encoder.phi)
      //
      "" //
  );
}
