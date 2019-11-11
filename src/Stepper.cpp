#include "Stepper.h"
#include "pinDef.h"

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
  TIMSK1 &= ~(1 << OCIE1A);         // Disable timer compare interrupt
  interrupts();                     // enable all interrupts

  return setMicroSteps(microStep) && setDirection(direction);
}

void Stepper::step()
{
  if ((sleepPin & PORTD) && (targetSteps - currSteps))
  {
    // Serial.print("");
    if (stepStatus)
    {
      PORTD |= stepPin;
    }
    else
    {
      PORTD &= ~stepPin;
      currSteps++;
    }
    stepStatus = !stepStatus;
    currTime = millis();
  }
}

uint32_t Stepper::getSteps(bool select)
{
  if (select)
    return currSteps;
  else
    return targetSteps;
}

double Stepper::getRevolutions(bool select)
{
  if (select)
    // Current stepper motor revolutions
    return currSteps / ((double)stepPerRev * (double)microStep);
  else
    // Target stepper motor Revolutions
    return targetSteps / ((double)stepPerRev * (double)microStep);
}

double Stepper::getDistance(bool select)
{
  if (select)
    // Current  distance
    return getRevolutions(1) * distPerRev;
  else
    return targetDistance;
}

unsigned long Stepper::getTime(bool select)
{
  if (select)
    // Start time
    return startTime;
  else
    // Current time
    return millis();
}

unsigned long Stepper::getRunTime()
{

  runTime = (currTime - startTime);

  return runTime / 1000;
}

bool Stepper::setDistance(double dist, bool select)
{
  if (select)
  // Current distance
  {
    currDistance = dist;
    // update steps to match
    currSteps = step2um_Mult * microStep * currDistance;
    return true;
  }
  else
  // Target distance
  {
    targetDistance = dist;
    // Update steps to match
    targetSteps = step2um_Mult * microStep * targetDistance;
    return true;
  }
  return false;
}

bool Stepper::setStartTime()
{
  currTime = millis();
  Serial.println(startTime);
  if (!startTime)
  {
    Serial.println(startTime);
    startTime = millis();
  }
  else
  {
    startTime = currTime - runTime;
    Serial.println(startTime);
  }

  return true;
}

bool Stepper::setTargetDistance(double distance)
{
  double tempDouble = (distance * stepPerRev * microStep) / distPerRev;
  targetSteps = (uint32_t)tempDouble;
  return true;
}

bool Stepper::setMicroSteps(uint8_t microSteps)
{
  switch (microSteps)
  {
  case 1:
    PORTD &= ~m0Pin | ~m1Pin;
    break;

  case 2:
    PORTD |= m0Pin;
    PORTD &= ~m1Pin;
    break;

  case 8:
    PORTD &= ~m0Pin;
    PORTD |= m1Pin;
    break;

  case 16:
    PORTD |= m0Pin | m1Pin;
    break;

  default:
    return false;
    break;
  }

  return true;
};

bool Stepper::setDirection(bool dir)
{
  if (dir)
    PORTD |= dirPin;
  else
    PORTD &= ~dirPin;

  return true;
}

bool Stepper::reset()
{
  currSteps = 0;
  targetSteps = 0;
  currDistance = 0;   // Stored in um
  targetDistance = 0; // Stored in um
  stepStatus = 0;
  startTime = 0;
  currTime = 0;
  runTime = 0;

  return true;
}