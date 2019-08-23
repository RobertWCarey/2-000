#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h
#include "CmdInterface.h"
#include "Stepper.h"

// UART Baud rate
static const uint32_t BAUD_RATE = 9600;

// interrupt frequency in Hz ie 2x number of steps
const float sampleRate = 400.0f;

CmdInterface cmdInterface;

Stepper drv8834;

void setup()
{

  drv8834.dirPin = DIR_PIN;
  drv8834.stepPin = STEP_PIN;
  drv8834.sleepPin = SLEEP_PIN;
  drv8834.m0Pin = M0_PIN;
  drv8834.m1Pin = M1_PIN;
  drv8834.microStep = 16;
  drv8834.direction = 1;

  drv8834.init(sampleRate);
  DDRB |= (1 << PORTB5);
  PORTB |= (1 << PORTB5);

  // Start Serial Port
  Serial.begin(BAUD_RATE);

  Serial.println(drv8834.stepPin);
  Serial.println(DDRD);
}

int spd = 0xffff;
int clk = 0;

// timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
  drv8834.step();
}

void loop()
{
  // revolutions = steps / 1600;
  // distance = revolutions * 0.095;

  cmdInterface.getCommandLineFromSerialPort(); //global CommandLine is defined in CommandLine.h
}
