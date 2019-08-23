#ifndef stepper_h
#define stepper_h

#include <string.h>
#include <stdlib.h>
#include "Arduino.h"

class Stepper
{

private:
  uint8_t steps = 0;
  uint8_t portAddrs;
  uint8_t dataDirAddrs;

public:
  // All pins should be on the same port
  uint8_t dirPin;
  uint8_t stepPin;
  uint8_t sleepPin;
  uint8_t m0Pin;
  uint8_t m1Pin;
  uint8_t port;
  uint8_t microStep;
  bool direction;
  bool init(float sampleRate);
  double getRevolutions();
  double getDistance();
  double getRunTime();
  bool setTargetDistance();
  bool setMicroSteps(uint8_t microSteps);
  bool setDirection(bool dir);
};

#endif // stepper_h