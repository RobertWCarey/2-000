/** \addtogroup CmdProt_module Command Protocol module documentation
 * @{
 */
#include "CmdInterface.h"

CmdInterface::CmdInterface(Stepper *stepper)
{
  cmdStepper = stepper;
  // return true;
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
    cmdStepper->setStartTime();
    SLEEP_PIN_HIGH;
    TIMSK1 |= (1 << OCIE1A);
    return true;
  }
  else if (!firstOperand)
  {
    SLEEP_PIN_LOW;
    TIMSK1 &= ~(1 << OCIE1A);
    return true;
  }
  return false;
}

bool CmdInterface::getSummary()
{
  uint16_t key = 0;
  term.cls();
  while (key != ' ')
  {
    term.position(0, 0);
    term.set_attribute(BT_NORMAL);
    term.set_attribute(BT_UNDERLINE);
    term.println(F("Summary"));

    // Target Distance
    term.set_attribute(BT_NORMAL);
    term.print(F("  Target Distance: "));
    term.set_attribute(BT_BLINK);
    term.print(cmdStepper->getDistance(0));
    term.println(F(" um"));

    // Distance Covered
    term.set_attribute(BT_NORMAL);
    term.print(F("  Current Distance: "));
    term.set_attribute(BT_BLINK);
    term.print(cmdStepper->getDistance(1));
    term.println(F(" um"));

    // Current Runtime
    term.set_attribute(BT_NORMAL);
    term.print(F("  Current Runtime: "));
    term.set_attribute(BT_BLINK);
    term.print(cmdStepper->getRunTime());
    term.println(F(" s"));

    term.println();

    // Start time
    term.set_attribute(BT_NORMAL);
    term.print(F("  Start Time: "));
    term.set_attribute(BT_BLINK);
    term.print(cmdStepper->getTime(1));
    term.println(F(" ms"));

    // Current time
    term.set_attribute(BT_NORMAL);
    term.print(F("  Current Time: "));
    term.set_attribute(BT_BLINK);
    term.print(cmdStepper->getTime(0));
    term.println(F(" ms"));

    // Target Revolutions
    term.set_attribute(BT_NORMAL);
    term.print(F("  Target Revolutions: "));
    term.set_attribute(BT_BLINK);
    term.println(cmdStepper->getRevolutions(0));

    // Current Revolutions
    term.set_attribute(BT_NORMAL);
    term.print(F("  Current Revolutions: "));
    term.set_attribute(BT_BLINK);
    term.println(cmdStepper->getRevolutions(1));

    // Target Steps
    term.set_attribute(BT_NORMAL);
    term.print(F("  Target Steps: "));
    term.set_attribute(BT_BLINK);
    term.println(cmdStepper->getSteps(0));

    // Current Steps
    term.set_attribute(BT_NORMAL);
    term.print(F("  Current Steps: "));
    term.set_attribute(BT_BLINK);
    term.println(cmdStepper->getSteps(1));

    key = term.get_key();
    switch (key)
    {
    case '\f':
      /* Ctrl-L: redraw screen */
      term.cls();
      break;
    }
  }

  // Set font back to default before exit
  term.set_attribute(BT_NORMAL);

  return true;
}

bool CmdInterface::setDistance()
{
  char *option = readWord();
  int dist = readNumber();
  // char *unit = readWord();

  if (!strcasecmp(option, "-t"))
  {
    cmdStepper->setDistance((double)dist, 0);
    return true;
  }
  else if (!strcasecmp(option, "-c"))
  {
    cmdStepper->setDistance((double)dist, 1);
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
    printHelpCmd(startCmd.cmd, startCmd.description, startCmd.params);
    printHelpCmd(getSummaryCmd.cmd, getSummaryCmd.description, getSummaryCmd.params);
    printHelpCmd(setDisCmd.cmd, setDisCmd.description, setDisCmd.params);
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

void CmdInterface::printHelpCmd(const String cmd, const String description, const String paramters[])
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