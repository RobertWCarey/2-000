#include "Stepper.h"

bool Stepper::init(float sampleRate)
{
  //Set data direction to OUTPUT
  DDRD |= dirPin | stepPin | sleepPin | m0Pin | m1Pin;

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

  return setMicroSteps(microStep) && setDirection(direction);
}

void Stepper::step()
{
  if (sleepPin & PORTD)
  {
    if (stepStatus)
    {
      PORTD |= stepPin;
    }
    else
    {
      PORTD &= ~stepPin;
      currSteps++;
      // Serial.println(stepPin);
    }
    stepStatus = !stepStatus;
  }
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
    PORTD |= ~m0Pin | ~m1Pin;
    break;

  case 2:
    PORTD |= m0Pin | ~m1Pin;
    break;

  case 8:
    PORTD |= ~m0Pin | m1Pin;
    break;

  case 16:
    PORTD |= m0Pin | m1Pin;
    break;

  default:
    return false;
    break;
  }

  return true;
}

bool Stepper::setDirection(bool dir)
{
  if (dir)
    PORTD |= dirPin;
  else
    PORTD |= ~dirPin;

  return true;
}