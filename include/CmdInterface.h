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

class CmdInterface
{
  char CommandLine[COMMAND_BUFFER_LENGTH + 1]; //Read commands into this buffer from Serial.  +1 in length for a termination char
  double distance = 0;
  const char *delimiters = ", \n"; // commands can be separated by return, space or comma
  const char *distanceCmdToken = "Distance";
  const char *sleepCmdToken = "Sleep";

private:
  void doMyCommand();
  bool sleepCommand();
  char *readWord();
  int readNumber();
  int strcicmp(char const *a, char const *b);
  void nullCommand(char *ptrToCommandName);

public:
  void getCommandLineFromSerialPort();
};

/**
 * @brief 
 * 
 * @return char* 
 */

#endif // cmdInterface_h

/** @} */
