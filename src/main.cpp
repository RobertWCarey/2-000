#include "Arduino.h"
#include "CommandLine.h"

// DRV8834 pins definitions
static const uint8_t DIR_PIN = 1 << PORTD2;
static const uint8_t STEP_PIN = 1 << PORTD3;
static const uint8_t M0_PIN = 1 << PORTD5;
static const uint8_t M1_PIN = 1 << PORTD6;

// Define commands for setting DRV8834 pins
#define DIR_PIN_HIGH PORTD |= DIR_PIN
#define DIR_PIN_LOW PORTD &= ~DIR_PIN
#define STEP_PIN_HIGH PORTD |= STEP_PIN
#define STEP_PIN_LOW PORTD &= ~STEP_PIN
#define M0_HIGH PORTD |= M0_PIN
#define M0_LOW PORTD &= ~M0_PIN
#define M1_HIGH PORTD |= M1_PIN
#define M1_LOW PORTD &= ~M1_PIN

char CommandLine[COMMAND_BUFFER_LENGTH + 1]; //Read commands into this buffer from Serial.  +1 in length for a termination char

double distance = 0;

// interrupt frequency in Hz ie 2x number of steps
const float samplerate = 10000.0f;

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

  Serial.begin(115200);
  uint8_t temp = ~(1 << PORTD2);
  Serial.println(temp);
}

uint32_t wait = 0;
static uint32_t steps = 0;

static double revolutions = 0;
int spd = 0xffff;
int clk = 0;

// timer 1 interrupt
ISR(TIMER1_COMPA_vect)
{
  // generate the next clock pulse on accumulator overflow
  // wait += spd;
  // if (wait >= 0xffff) {
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

bool parseCommand(String com);

String command;

void loop()
{
  revolutions = steps / 1600;
  distance = revolutions * 0.095;

  bool received = getCommandLineFromSerialPort(CommandLine); //global CommandLine is defined in CommandLine.h
  if (received)
    DoMyCommand(CommandLine, &distance);
}
