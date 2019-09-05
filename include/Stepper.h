#ifndef stepper_h
#define stepper_h

#include <string.h>
#include <stdlib.h>
#include "Arduino.h"

const double distPerRev PROGMEM = 0.008; // Distance in m

class Stepper
{

#define step2um_Mult 2100
#define stepPerRev 200

private:
  uint32_t currSteps = 0;
  uint32_t targetSteps;
  double currDistance = 0;   // Stored in um
  double targetDistance = 0; // Stored in um
  bool stepStatus = 0;
  unsigned long startTime = 0;

public:
  // All pins have to be on PORTD
  uint8_t dirPin;
  uint8_t stepPin;
  uint8_t sleepPin;
  uint8_t m0Pin;
  uint8_t m1Pin;
  uint8_t port;
  uint8_t microStep;
  bool direction;
  void step();
  bool init(float sampleRate);
  uint32_t getSteps(bool select);
  double getRevolutions(bool select);
  double getDistance(bool select);
  unsigned long getTime(bool select);
  unsigned long getRunTime();
  bool setDistance(double dist, bool select);
  bool setStartTime();
  bool setTargetDistance(double distance);
  bool setMicroSteps(uint8_t microSteps);
  bool setDirection(bool dir);
};

#endif // stepper_h