#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h
#include "CmdInterface.h"
#include "Stepper.h"
#include "LiquidCrystal_I2C.h"

#define SW_VERSION 1.00

// UART Baud rate
static const uint32_t BAUD_RATE = 115200;

// interrupt frequency in Hz ie 2x number of steps
const float sampleRate = 14000.0f;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

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
  drv8834.microStep = 8;
  drv8834.direction = 1;

  // Initilise the stepper
  // Ensure configuration has been set
  drv8834.init(sampleRate);

  lcd.init();
  lcd.backlight();     //light this binch up
  lcd.setCursor(0, 0); //prints on first/top line idiot
  lcd.print(F("Cell Stretcher"));
  lcd.setCursor(0, 1);
  lcd.print(F("SW: V"));
  lcd.print(SW_VERSION);
  delay(2000);
  lcd.clear();
}

// timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
  drv8834.step();
}

void Print2(int n)
{
  if (n < 10)
    lcd.print('0');
  lcd.print(n);
}

void updateLCD()
{
  lcd.setCursor(0, 0);
  lcd.print(F("Distance:"));
  lcd.print(drv8834.getDistance(1));
  lcd.setCursor(0, 1);
  lcd.print(F("RunTime:"));
  unsigned long Now = drv8834.getRunTime();
  int Seconds = Now % 60;
  int Minutes = (Now / 60) % 60;
  int Hours = (Now / 3600) % 24;
  Print2(Hours);
  lcd.print(':');
  Print2(Minutes);
  lcd.print(':');
  Print2(Seconds);
}

void loop()
{
  drv8834Cmd.getCommandLineFromSerialPort(); //global CommandLine is defined in CommandLine.h
  updateLCD();
}
