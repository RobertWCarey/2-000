#ifndef add_h
#define add_h

#include <string.h>
#include <stdlib.h>
#include "Arduino.h"

#define print2(x,y) (Serial.print(x), Serial.println(y))


#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '

#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer for incoming commands

#define SLEEP_PIN_HIGH PORTD |= 0b00100000
#define SLEEP_PIN_LOW PORTD &= 0b11011111

int readNumber ();

char * readWord();

void nullCommand(char * ptrToCommandName);

int addCommand();

int subtractCommand();

bool sleepCommand();

void DoMyCommand(char * commandLine, double * distance);

bool getCommandLineFromSerialPort(char * commandLine);

#endif
