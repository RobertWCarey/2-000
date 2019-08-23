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
#define MAX_NUM_PARAMS 5

typedef struct
{
  const char *cmd;               /*!< Command token that triggers cmd */
  String description;            /*!< Description of command */
  String params[MAX_NUM_PARAMS]; /*!< parameter that the command responds to*/
} Command;

static Command sleepCmd = {"Sleep", "Enables/Disables Stepper Driver", {"'1' to enable stepper", "'0' to disable stepper"}};
static Command helpCmd = {"-h", "Displays a summary of commands", {"NULL"}};

static const char *delimiters = ", \n"; // commands can be separated by return, space or comma

class CmdInterface
{
  char CommandLine[COMMAND_BUFFER_LENGTH + 1]; //Read commands into this buffer from Serial.  +1 in length for a termination char

private:
  void doMyCommand();
  bool sleepCommand();
  char *readWord();
  int readNumber();
  int strcicmp(char const *a, char const *b);
  void nullCommand(char *ptrToCommandName);
  void printHelpCmd(char const *cmd, String description, const String paramters[]);

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
