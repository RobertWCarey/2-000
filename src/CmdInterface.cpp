/** \addtogroup CmdProt_module Command Protocol module documentation
 * @{
 */
#include "CmdInterface.h"

int CmdInterface::strcicmp(char const *a, char const *b)
{
  for (;; a++, b++)
  {
    int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
    if (d != 0 || !*a)
      return d;
  }
}

int CmdInterface::readNumber()
{
  char *numTextPtr = strtok(NULL, delimiters); //K&R string.h  pg. 250
  return atoi(numTextPtr);                     //K&R string.h  pg. 251
}

char *CmdInterface::readWord()
{
  char *word = strtok(NULL, delimiters); //K&R string.h  pg. 250
  return word;
}

void CmdInterface::nullCommand(char *ptrToCommandName)
{
  Serial.print("Command not found: ");
  Serial.println(ptrToCommandName);
}

bool CmdInterface::sleepCommand()
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

void CmdInterface::doMyCommand()
{
  //  print2("\nCommand: ", commandLine);
  double result;

  char *ptrToCommandName = strtok(CommandLine, delimiters);
  //  print2("commandName= ", ptrToCommandName);

  if (strcicmp(ptrToCommandName, distanceCmdToken) == 0)
  {
    Serial.print(">    The Distance travelled is = ");
    Serial.print(result);
    Serial.println(" um");
  }
  else if (strcicmp(ptrToCommandName, sleepCmdToken) == 0)
  {
    if (sleepCommand())
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