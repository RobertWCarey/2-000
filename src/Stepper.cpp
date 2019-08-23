#include "Stepper.h"

bool Stepper::init(float sampleRate)
{
  switch (port)
  {
  case 3:
    portAddrs = PORTD;
    dataDirAddrs = DDRD;
    break;

  default:
    break;
  }

  //Set data direction to OUTPUT
  dataDirAddrs |= dirPin | stepPin | sleepPin | m0Pin | m1Pin;

  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 16000000.0f / sampleRate; // compare match register for IRQ with selected samplerate
  TCCR1B |= (1 << WGM12);           // CTC mode
  TCCR1B |= (1 << CS10);            // no prescaler
  TIMSK1 |= (1 << OCIE1A);          // enable timer compare interrupt
  interrupts();                     // enable all interrupts

  // Default full step
  portAddrs |= dirPin | ~m0Pin | ~m1Pin;

  return true;
}

double Stepper::getRevolutions()
{

  return 1.0;
}

double Stepper::getDistance()
{

  return 1.0;
}

double Stepper::getRunTime()
{

  return 1.0;
}

bool Stepper::setTargetDistance()
{

  return false;
}

bool Stepper::setMicroSteps(uint8_t microSteps)
{
  switch (microSteps)
  {
  case 1:
    portAddrs |= ~m0Pin | ~m1Pin;
    break;

  case 2:
    portAddrs |= m0Pin | ~m1Pin;
    break;

  case 8:
    portAddrs |= ~m0Pin | m1Pin;
    break;

  case 16:
    portAddrs |= m0Pin | m1Pin;
    break;

  default:
    return false;
    break;
  }

  return true;
}