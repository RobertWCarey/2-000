#include "CommandLine.h"


const char *delimiters            = ", \n";                    //commands can be separated by return, space or comma
const char *addCommandToken       = "add";                     //Modify here
const char *subtractCommandToken  = "sub";                     //Modify here
const char *ditanceCmdToken       = "dist";

int readNumber ()
{
  char * numTextPtr = strtok(NULL, delimiters);         //K&R string.h  pg. 250
  return atoi(numTextPtr);                              //K&R string.h  pg. 251
}

char * readWord()
{
  char * word = strtok(NULL, delimiters);               //K&R string.h  pg. 250
  return word;
}

void nullCommand(char * ptrToCommandName)
{
  print2("Command not found: ", ptrToCommandName);      //see above for macro print2
}

int addCommand()
{                                      //Modify here
  int firstOperand = readNumber();
  int secondOperand = readNumber();
  return firstOperand + secondOperand;
}

int subtractCommand()
{                                //Modify here
  int firstOperand = readNumber();
  int secondOperand = readNumber();
  return firstOperand - secondOperand;
}

int distanceCommand()
{                                //Modify here
  int distance = 1;
  return distance;
}

void DoMyCommand(char * commandLine)
{
  //  print2("\nCommand: ", commandLine);
  int result;

  char * ptrToCommandName = strtok(commandLine, delimiters);
  //  print2("commandName= ", ptrToCommandName);

  if (strcmp(ptrToCommandName, addCommandToken) == 0)
  {                   //Modify here
    result = addCommand();
    print2(">    The sum is = ", result);

  }
  else if (strcmp(ptrToCommandName, subtractCommandToken) == 0)
  {
    result = subtractCommand();                                       //K&R string.h  pg. 251
    print2(">    The difference is = ", result);
  }
  else if (strcmp(ptrToCommandName, ditanceCmdToken) == 0)
  {
    result = distanceCommand();                                       //K&R string.h  pg. 251
    print2(">    The Distance travelled is = ", result);
  }
  else
  {
    nullCommand(ptrToCommandName);
  }
}

bool getCommandLineFromSerialPort(char * commandLine)
{
  static uint8_t charsRead = 0;                      //note: COMAND_BUFFER_LENGTH must be less than 255 chars long
  //read asynchronously until full command input
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:      //likely have full command in buffer now, commands are terminated by CR and/or LS
      case LF:
        commandLine[charsRead] = NULLCHAR;       //null terminate our command char array
        if (charsRead > 0)  {
          charsRead = 0;                           //charsRead is static, so have to reset
          Serial.println(commandLine);
          return true;
        }
        break;
      case BS:                                    // handle backspace in input: put a space in last char
        if (charsRead > 0) {                        //and adjust commandLine and charsRead
          commandLine[--charsRead] = NULLCHAR;
          Serial << byte(BS) << byte(SPACE) << byte(BS);  //no idea how this works, found it on the Internet
        }
        break;
      default:
        // c = tolower(c);
        if (charsRead < COMMAND_BUFFER_LENGTH) {
          commandLine[charsRead++] = c;
        }
        commandLine[charsRead] = NULLCHAR;     //just in case
        break;
    }
  }
  return false;
}
