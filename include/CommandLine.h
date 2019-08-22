#ifndef cmdInterface_h
#define cmdInterface_h

#include <string.h>
#include <stdlib.h>
#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '
#define COMMAND_BUFFER_LENGTH 25 //length of serial buffer for incoming commands

#define print2(x, y) (Serial.print(x), Serial.println(y))
#define print3(x, y, z) (Serial.print(x), Serial.print(y), Serial.println(z))

class CmdInterface
{
  char CommandLine[COMMAND_BUFFER_LENGTH + 1]; //Read commands into this buffer from Serial.  +1 in length for a termination char
  double distance = 0;

private:
  void doMyCommand();
  void printStrings(const String *strings, int numStrings);

public:
  void getCommandLineFromSerialPort();
};

int readNumber();

/**
 * @brief 
 * 
 * @return char* 
 */
char *readWord();

void nullCommand(char *ptrToCommandName);

/**
 * @brief Excutes the command to turn the stepper contoller sleep pin high/low
 * 
 * @return true 
 * @return false 
 */
bool sleepCommand();

/**
 * @brief Parses command 
 * 
 * @param commandLine 
 * @param distance 
 */

#endif // cmdInterface_h

/** @} */
