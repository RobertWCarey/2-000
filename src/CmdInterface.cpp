/** \addtogroup CmdProt_module Command Protocol module documentation
 * @{
 */
#include "CmdInterface.h"

bool CmdInterface::init(Stepper &stepper)
{
  cmdStepper = stepper;
  return true;
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
  Serial.print(F("Command not found: "));
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
  Serial.print(F("  Target Distance: "));
  Serial.println(cmdStepper.getDistance(0));
  // Distance Covered
  Serial.print(F("  Current Distance: "));
  Serial.println(cmdStepper.getDistance(1));
  // Current Runtime
  Serial.print(F("  Current Runtime: "));
  Serial.println(cmdStepper.getRunTime());
  char *option = readWord();
  if (!strcasecmp(option, "-e"))
  {
    // Start time
    Serial.print(F("  Start Time: "));
    Serial.println(cmdStepper.getTime(1));
    // Current time
    Serial.print(F("  Current Time: "));
    Serial.println(cmdStepper.getTime(0));
    // Target Revolutions
    Serial.print(F("  Target Revolutions: "));
    Serial.println(cmdStepper.getRevolutions(0));
    // Current Revolutions
    Serial.print(F("  Current Revolutions: "));
    Serial.println(cmdStepper.getRevolutions(1));
    // Target Steps
    Serial.print(F("  Traget Steps: "));
    Serial.println(cmdStepper.getSteps(1));
    // Current Steps
    Serial.print(F("  Current Steps: "));
    Serial.println(cmdStepper.getSteps(0));
  }
  return true;
}

bool CmdInterface::setDistance()
{
  char *option = readWord();
  int dist = readNumber();
  // char *unit = readWord();

  if (!strcasecmp(option, "-t"))
  {
    cmdStepper.setDistance((double)dist, 0);
    return true;
  }
  else if (!strcasecmp(option, "-c"))
  {
    cmdStepper.setDistance((double)dist, 1);
    return true;
  }

  return false;
}

void CmdInterface::doMyCommand()
{
  char *ptrToCommandName = strtok(CommandLine, delimiters);

  if (!strcasecmp(ptrToCommandName, startCmd.cmd.c_str()))
  {
    if (startCommand())
      Serial.println(F(">    The start setting was set"));
    else
      Serial.println(F(">    The start setting was not set"));
  }
  else if (!strcasecmp(ptrToCommandName, helpCmd.cmd.c_str()))
  {
    Serial.println(F("Commands:"));
    printHelpCmd(startCmd.cmd.c_str(), startCmd.description, startCmd.params);
    printHelpCmd(getSummaryCmd.cmd.c_str(), getSummaryCmd.description, getSummaryCmd.params);
    printHelpCmd(setDisCmd.cmd.c_str(), setDisCmd.description, setDisCmd.params);
  }
  else if (!strcasecmp(ptrToCommandName, getSummaryCmd.cmd.c_str()))
  {
    getSummary();
  }
  else if (!strcasecmp(ptrToCommandName, setDisCmd.cmd.c_str()))
  {
    if (setDistance())
      Serial.println(F(">   The distance setting was set"));
    else
      Serial.println(F(">   The distance setting was not set"));
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
    Serial.println(F("    Params:"));
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