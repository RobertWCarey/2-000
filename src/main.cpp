#include "Arduino.h"
#include "DRV8834.h"
#include "HotStepper.h"

#define DIR_PIN_HIGH PORTD |= 0b00001000
#define DIR_PIN_LOW PORTD &= 0b11110111

#define STEP_PIN_HIGH PORTD |= 0b00010000
#define STEP_PIN_LOW PORTD &= 0b11101111

const int analogPin = A0;

// interrupt frequency
const float samplerate = 10000.0f;

void setup() {
  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 16000000.0f / samplerate; // compare match register for IRQ with selected samplerate
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS10); // no prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
  DIR_PIN_HIGH;
}

uint32_t wait = 0;
int spd = 0xffff;
int clk = 0;

// timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
  // generate the next clock pulse on accumulator overflow
  wait += spd;
  if (wait >= 0xffff) {
    if (clk) {
      STEP_PIN_HIGH;
    } else {
      STEP_PIN_LOW;
    }
    wait -= 0x10000;
    clk = !clk;
  }
}

void loop() {
  // // read potentiometer and set direction
  // int poti = analogRead(analogPin);
  // if (poti < 512) {
  //     // digitalWrite(dirPin, LOW);
  //     DIR_PIN_HIGH;
  //     poti = 512 - poti;
  // } else {
  //     // digitalWrite(dirPin, HIGH);
  //     DIR_PIN_LOW;
  //     poti -= 512;
  // }
  //
  // // update the speed, which increments the accumulator in the interrupt
  // noInterrupts();
  // spd = poti * 10;
  // interrupts();
}
