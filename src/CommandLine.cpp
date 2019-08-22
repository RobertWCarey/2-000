/** \addtogroup CmdProt_module Command Protocol module documentation
 * @{
 */
#include "CommandLine.h"

const char *delimiters = ", \n"; // commands can be separated by return, space or comma
const char *distanceCmdToken = "Distance";
const char *sleepCmdToken = "Sleep";

static int strcicmp(char const *a, char const *b)
{
  for (;; a++, b++)
  {
    int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
    if (d != 0 || !*a)
      return d;
  }
}

int readNumber()
{
  char *numTextPtr = strtok(NULL, delimiters); //K&R string.h  pg. 250
  return atoi(numTextPtr);                     //K&R string.h  pg. 251
}

char *readWord()
{
  char *word = strtok(NULL, delimiters); //K&R string.h  pg. 250
  return word;
}

void nullCommand(char *ptrToCommandName)
{
  print2("Command not found: ", ptrToCommandName); //see above for macro print2
}

bool sleepCommand()
{
  int firstOperand = readNumber();
  if (firstOperand == 1)
  {
    SLEEP_PIN_HIGH;
    return true;
  }
  else if (!firstOperand)
  {
    SLEEP_PIN_LOW;
    return true;
  }
  return false;
}

void CmdInterface::printStrings(const String *strings, int numStrings)
{
  String string;
  for (int i = 0; i < numStrings - 1; i++)
  {
    string = *strings;
    Serial.print(string);
    strings++;
  }
}

void CmdInterface::doMyCommand()
{
  //  print2("\nCommand: ", commandLine);
  double result;

  char *ptrToCommandName = strtok(CommandLine, delimiters);
  //  print2("commandName= ", ptrToCommandName);

  if (strcicmp(ptrToCommandName, distanceCmdToken) == 0)
  {
    result = distance; //K&R string.h  pg. 251
    print3(">    The Distance travelled is = ", result, " um");
  }
  else if (strcicmp(ptrToCommandName, sleepCmdToken) == 0)
  {
    result = sleepCommand();
    if (result)
      Serial.println(">    The Sleep setting was set");
    else
      Serial.println(">    The Sleep setting was not set");
  }
  else
  {
    nullCommand(ptrToCommandName);
  }
}

void CmdInterface::getCommandLineFromSerialPort()
{
  static uint8_t charsRead = 0; //note: COMAND_BUFFER_LENGTH must be less than 255 chars long
  //read asynchronously until full command input
  while (Serial.available())
  {
    char c = Serial.read();
    switch (c)
    {
    case CR: // Commands are terminated by CR and/or LS
    case LF:
      CommandLine[charsRead] = NULLCHAR; //null terminate our command char array
      if (charsRead > 0)
      {
        charsRead = 0; //charsRead is static, so have to reset
        Serial.println();
        doMyCommand();
      }
      break;
    case BS: // handle backspace in input: put a space in last char
      if (charsRead > 0)
      { //and adjust CommandLine and charsRead
        CommandLine[--charsRead] = NULLCHAR;
        Serial.print(BS);
        Serial.print(' '); // Char in Terminal is overridden with a space
        Serial.print(BS);
      }
      break;
    default:
      Serial.print(c);
      c = tolower(c);
      if (charsRead < COMMAND_BUFFER_LENGTH)
      {
        // Adds character to array and then increments charsRead
        CommandLine[charsRead++] = c;
      }
      // Clear next position in array for safety
      CommandLine[charsRead] = NULLCHAR;
      break;
    }
  }
}

/** @} */