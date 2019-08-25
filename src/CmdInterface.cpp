/** \addtogroup CmdProt_module Command Protocol module documentation
 * @{
 */
#include "CmdInterface.h"

bool CmdInterface::init(Stepper &stepper)
{
  cmdStepper = stepper;
  return true;
}

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

bool CmdInterface::startCommand()
{
  int firstOperand = readNumber();
  if (firstOperand == 1)
  {
    cmdStepper.setStartTime();
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

bool CmdInterface::getSummary()
{
  // Target Distance
  Serial.print("  Target Distance: ");
  Serial.println(cmdStepper.getDistance(0));
  // Distance Covered
  Serial.print("  Current Distance: ");
  Serial.println(cmdStepper.getDistance(1));
  // Current Runtime
  Serial.print("  Current Runtime: ");
  Serial.println(cmdStepper.getRunTime());
  char *option = readWord();
  if (!strcicmp(option, "-e"))
  {
    // Target Revolutions
    Serial.print("  Target Revolutions: ");
    Serial.println(cmdStepper.getRevolutions(0));
    // Current Revolutions
    Serial.print("  Current Revolutions: ");
    Serial.println(cmdStepper.getRevolutions(1));
    // Target Steps
    Serial.print("  Traget Steps: ");
    Serial.println(cmdStepper.getSteps(1));
    // Current Steps
    Serial.print("  Current Steps: ");
    Serial.println(cmdStepper.getSteps(0));
  }
  return true;
}

bool CmdInterface::setDistance()
{
  char *option = readWord();
  int dist = readNumber();
  // char *unit = readWord();

  if (!strcicmp(option, "-t"))
  {
    cmdStepper.setDistance((double)dist, 0);
    return true;
  }
  else if (!strcicmp(option, "-c"))
  {
    cmdStepper.setDistance((double)dist, 1);
    return true;
  }

  return false;
}

void CmdInterface::doMyCommand()
{
  char *ptrToCommandName = strtok(CommandLine, delimiters);

  if (!strcicmp(ptrToCommandName, startCmd.cmd))
  {
    if (startCommand())
      Serial.println(">    The start setting was set");
    else
      Serial.println(">    The start setting was not set");
  }
  else if (!strcicmp(ptrToCommandName, helpCmd.cmd))
  {
    Serial.println("Commands:");
    printHelpCmd(startCmd.cmd, startCmd.description, startCmd.params);
    printHelpCmd(getSummaryCmd.cmd, getSummaryCmd.description, getSummaryCmd.params);
    printHelpCmd(setDisCmd.cmd, setDisCmd.description, setDisCmd.params);
  }
  else if (!strcicmp(ptrToCommandName, getSummaryCmd.cmd))
  {
    getSummary();
  }
  else if (!strcicmp(ptrToCommandName, setDisCmd.cmd))
  {
    if (setDistance())
      Serial.println(">   The distance setting was set");
    else
      Serial.println(">   The distance setting was not set");
  }
  else
  {
    nullCommand(ptrToCommandName);
  }
}

void CmdInterface::printHelpCmd(char const *cmd, String description, const String paramters[])
{
  int numParams = sizeof(paramters);
  // Serial.println(numParams);

  Serial.print("  ");
  Serial.print("'");
  Serial.print(cmd);
  Serial.print("'");
  Serial.print("  ");
  Serial.println(description);

  if (paramters[0] != "NULL")
  {
    Serial.println("    Params:");
    for (int i = 0; i < numParams; i++)
    {
      Serial.print("      ");
      Serial.println(paramters[i]);
    }
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