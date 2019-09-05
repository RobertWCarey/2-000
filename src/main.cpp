#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h
#include "CmdInterface.h"
#include "Stepper.h"
#include "BasicTerm.h"

// UART Baud rate
static const uint32_t BAUD_RATE = 115200;

// interrupt frequency in Hz ie 2x number of steps
const float sampleRate = 400.0f;

Stepper drv8834;

CmdInterface drv8834Cmd(&drv8834);

// BasicTerm term(&Serial);

void setup()
{
  // Start Serial Port
  Serial.begin(BAUD_RATE);

  // Configuration for stepper
  drv8834.dirPin = DIR_PIN;
  drv8834.stepPin = STEP_PIN;
  drv8834.sleepPin = SLEEP_PIN;
  drv8834.m0Pin = M0_PIN;
  drv8834.m1Pin = M1_PIN;
  drv8834.microStep = 16;
  drv8834.direction = 1;

  // Initilise the stepper
  // Ensure configuration has been set
  drv8834.init(sampleRate);
  // Initilise cmd interface for the stepper
  // drv8834Cmd.init(drv8834);
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

  // term.position(0, 0);
  // term.set_attribute(BT_NORMAL);
  // term.print(F("Current millis: "));
  // term.print(millis());

  drv8834Cmd.getCommandLineFromSerialPort(); //global CommandLine is defined in CommandLine.h
}
