#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h
#include "CmdInterface.h"

// UART Baud rate
static const uint32_t BAUD_RATE = 115200;

// interrupt frequency in Hz ie 2x number of steps
const float samplerate = 10000.0f;

CmdInterface cmdInterface;

void setup()
{

  //Set data direction to OUTPUT
  DDRD |= 0b01111100;

  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 16000000.0f / samplerate; // compare match register for IRQ with selected samplerate
  TCCR1B |= (1 << WGM12);           // CTC mode
  TCCR1B |= (1 << CS10);            // no prescaler
  TIMSK1 |= (1 << OCIE1A);          // enable timer compare interrupt
  interrupts();                     // enable all interrupts
  DIR_PIN_HIGH;
  M0_HIGH;
  M1_LOW;

  // Start Serial Port
  Serial.begin(BAUD_RATE);
}

uint32_t wait = 0;
static uint32_t steps = 0;

static double revolutions = 0;
int spd = 0xffff;
int clk = 0;

// timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
  if ((1 << PORTD4) & PORTD)
  {
    if (clk)
    {
      STEP_PIN_HIGH;
    }
    else
    {
      STEP_PIN_LOW;
      steps++;
    }
    // wait -= 0x10000;
    clk = !clk;
  }
  // }
}

void loop()
{
  revolutions = steps / 1600;
  // distance = revolutions * 0.095;

  cmdInterface.getCommandLineFromSerialPort(); //global CommandLine is defined in CommandLine.h
}
