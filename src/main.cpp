#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h
#include "CmdInterface.h"
#include "Stepper.h"

// UART Baud rate
static const uint32_t BAUD_RATE = 115200;

// interrupt frequency in Hz ie 2x number of steps
const float sampleRate = 10000.0f;

CmdInterface cmdInterface;

Stepper drv8834;

void setup()
{

  drv8834.dirPin = DIR_PIN;
  drv8834.stepPin = STEP_PIN;
  drv8834.sleepPin = SLEEP_PIN;
  drv8834.m0Pin = M0_PIN;
  drv8834.m1Pin = M1_PIN;
  drv8834.port = 3;
  drv8834.microStep = 16;

  drv8834.init(sampleRate);

  // Start Serial Port
  Serial.begin(BAUD_RATE);
}

uint32_t wait = 0;
static uint32_t steps = 0;

static double revolutions = 0;
int spd = 0xffff;
int clk = 0;

// timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
  if ((1 << PORTD4) & PORTD)
  {
    if (clk)
    {
      STEP_PIN_HIGH;
    }
    else
    {
      STEP_PIN_LOW;
      steps++;
    }
    // wait -= 0x10000;
    clk = !clk;
  }
  // }
}

void loop()
{
  revolutions = steps / 1600;
  // distance = revolutions * 0.095;

  cmdInterface.getCommandLineFromSerialPort(); //global CommandLine is defined in CommandLine.h
}
