#ifndef pinDef_h
#define pinDef_h

// Include this headr after Arduino.h
#include "stdint.h"

// DRV8834 pins definitions
static const uint8_t DIR_PIN = 1 << PORTD2;
static const uint8_t STEP_PIN = 1 << PORTD3;
static const uint8_t SLEEP_PIN = 1 << PORTD4;
static const uint8_t M0_PIN = 1 << PORTD5;
static const uint8_t M1_PIN = 1 << PORTD6;

// Define commands for setting DRV8834 pins
#define DIR_PIN_HIGH PORTD |= DIR_PIN
#define DIR_PIN_LOW PORTD &= ~DIR_PIN
#define STEP_PIN_HIGH PORTD |= STEP_PIN
#define STEP_PIN_LOW PORTD &= ~STEP_PIN
#define SLEEP_PIN_HIGH PORTD |= SLEEP_PIN
#define SLEEP_PIN_LOW PORTD &= ~SLEEP_PIN
#define M0_HIGH PORTD |= M0_PIN
#define M0_LOW PORTD &= ~M0_PIN
#define M1_HIGH PORTD |= M1_PIN
#define M1_LOW PORTD &= ~M1_PIN

#endif // pinDef_h