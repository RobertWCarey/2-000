#ifndef stepper_h
#define stepper_h

#include <string.h>
#include <stdlib.h>
#include "Arduino.h"

const double distPerRev = 0.008; // Distance in m

class Stepper
{

private:
  uint32_t currSteps = 0;
  uint32_t targetSteps;
  double currDistance;   // Stored in mm
  double targetDistance; // Stored in mm
  bool stepStatus = 0;
  unsigned long startTime;

public:
  // All pins have to be on PORTD
  uint8_t dirPin;
  uint8_t stepPin;
  uint8_t sleepPin;
  uint8_t m0Pin;
  uint8_t m1Pin;
  uint8_t port;
  uint8_t microStep;
  uint8_t stepPerRev;
  bool direction;
  void step();
  bool init(float sampleRate);
  int getSteps(bool select);
  double getRevolutions(bool select);
  double getDistance(bool select);
  double getRunTime();
  bool setDistance(double dist, bool select);
  bool setStartTime();
  bool setTargetDistance(double distance);
  bool setMicroSteps(uint8_t microSteps);
  bool setDirection(bool dir);
};

#endif // stepper_h