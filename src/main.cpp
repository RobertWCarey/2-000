#include "Arduino.h"
#include "DRV8834.h"
#include "HotStepper.h"
#include "CommandLine.h"
// #include "add.h"

#define DIR_PIN_HIGH PORTD |= 0b00001000
#define DIR_PIN_LOW PORTD &= 0b11110111

#define STEP_PIN_HIGH PORTD |= 0b00010000
#define STEP_PIN_LOW PORTD &= 0b11101111

char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

double distance = 0;

const int analogPin = A0;

// interrupt frequency in Hz ie 2x number of steps
const float samplerate = 14000.0f;

void setup() {
  // initialize timer1
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 16000000.0f / samplerate; // compare match register for IRQ with selected samplerate
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS10); // no prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
  DIR_PIN_HIGH;

  Serial.begin(115200);

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
    if (clk) {
      STEP_PIN_HIGH;
    } else {
      STEP_PIN_LOW;
      steps ++;
    }
    // wait -= 0x10000;
    clk = !clk;
  // }
}

bool parseCommand(String com);

String command;

void loop() {
  revolutions = steps/1600;
  distance = revolutions*0.095;

  bool received = getCommandLineFromSerialPort(CommandLine);      //global CommandLine is defined in CommandLine.h
  if (received)
    DoMyCommand(CommandLine, &distance);
  // read potentiometer and set direction
  // int poti = analogRead(analogPin);
  // if (poti < 512) {
  //     // digitalWrite(dirPin, LOW);
  //     DIR_PIN_HIGH;
  //     poti = 512 - poti;
  // } else {
  //     // digitalWrite(dirPin, HIGH);
  //     DIR_PIN_LOW;
  //     poti -= 512;
  // }
  //
  // // update the speed, which increments the accumulator in the interrupt
  // noInterrupts();
  // spd = poti * 10;
  // interrupts();

  // Serial.println(distance);
  // Serial.println("um");

  // if (Serial.available())
  // {
  //   char c = Serial.read();
  //   if (c == '\n')
  //   {
  //     if (!parseCommand(command))
  //     {
  //       // Serial.println("Invalid Input");
  //     }
  //     command = "";
  //   }
  //   else
  //   {
  //     command += c;
  //   }
  // }
}

// bool parseCommand(String com)
// {
//   String part1,part2;
//   float val;
//   noInterrupts();
//   part1 = com.substring(0, com.indexOf(' '));
//   part2 = com.substring(com.indexOf(' ')+1);
//
//   if (part1.equalsIgnoreCase("Distance"))
//   {
//     Serial.print("Distance: ");
//     Serial.print(distance);
//     Serial.println("um");
//     interrupts();
//     return true;
//   }
//   interrupts();
//   return false;
// }
