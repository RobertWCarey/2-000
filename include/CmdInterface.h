#ifndef cmdInterface_h
#define cmdInterface_h

#include <string.h>
#include <stdlib.h>
#include "Arduino.h"
#include "pinDef.h" // Call after Arduino.h
#include "Stepper.h"
#include "BasicTerm.h"

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '
#define COMMAND_BUFFER_LENGTH 25 //length of serial buffer for incoming commands
#define MAX_NUM_PARAMS 2

typedef struct
{
  String cmd;                    /*!< Command token that triggers cmd */
  String description;            /*!< Description of command */
  String params[MAX_NUM_PARAMS]; /*!< parameter that the command responds to*/
} Command;

const Command startCmd PROGMEM = {"start", "Enables/Disables Stepper Driver", {"'1' to enable stepper", "'0' to disable stepper"}};
const Command helpCmd PROGMEM = {"-h", "Displays a summary of commands", {"NULL"}};
const Command getSummaryCmd PROGMEM = {"summary", "Displays a summary of current state", {"'SPACE' to exit summary", "'ctrl + L' to redraw summary"}};
const Command setDisCmd PROGMEM = {"setDist", "Set current or target disatance", {"'-c' select current dist", "'-t' select target dist"}};
const Command resetCmd PROGMEM = {"reset", "Reset distance and runtime", {"'none'"}};

static const char *const delimiters PROGMEM = ", \n"; // commands can be separated by return, space or comma

static BasicTerm term(&Serial);

class CmdInterface
{
  char CommandLine[COMMAND_BUFFER_LENGTH + 1]; //Read commands into this buffer from Serial.  +1 in length for a termination char
  Stepper *cmdStepper;

private:
  void doMyCommand();
  bool startCommand();
  bool resetCommand();
  bool getSummary();
  bool setDistance();
  char *readWord();
  int readNumber();
  void nullCommand(char *ptrToCommandName);
  void printHelpCmd(const String cmd, const String description, const String paramters[]);

public:
  CmdInterface(Stepper *stepper);
  // bool init(Stepper &stepper);
  void getCommandLineFromSerialPort();
};

#endif // cmdInterface_h

/** @} */
