#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h
#include "CmdInterface.h"
#include "Stepper.h"

// UART Baud rate
static const uint32_t BAUD_RATE = 115200;

// interrupt frequency in Hz ie 2x number of steps
const float sampleRate = 400.0f;

CmdInterface drv8834Cmd;

Stepper drv8834;

void setup()
{
  // Configuration for stepper
  drv8834.dirPin = DIR_PIN;
  drv8834.stepPin = STEP_PIN;
  drv8834.sleepPin = SLEEP_PIN;
  drv8834.m0Pin = M0_PIN;
  drv8834.m1Pin = M1_PIN;
  drv8834.microStep = 16;
  drv8834.direction = 1;
  drv8834.stepPerRev = 200;

  // Initilise the stepper
  // Ensure configuration has been set
  drv8834.init(sampleRate);
  // Initilise cmd interface for the stepper
  drv8834Cmd.init(drv8834);

  // Start Serial Port
  Serial.begin(BAUD_RATE);
}

// timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
  drv8834.step();
}

void loop()
{
  // revolutions = steps / 1600;
  // distance = revolutions * 0.095;

  drv8834Cmd.getCommandLineFromSerialPort(); //global CommandLine is defined in CommandLine.h
}
