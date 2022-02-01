/*************************************************
 * @filename : cli_handler.c
 * @author   : Dhiraj Bennadi
 * @compiler : arm
 * @IDE      : MCUXpresso IDE
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */

#include "cli_handler.h"
#include "cli_commands.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

static void echoCharacter(char character);

/*
 * This function accumulates the characters until a return
 * key is pressed by the user on the terminal window
 *
 * Parameters: char *ipStrPtr: Pointer to the String
 *             int size: Length of the String
 *
 * Returns: None.
 */
void accumulateLine(char *ipStrPtr, int size)
{
    char recvByte;
    int location = 0;

    do
    {
      recvByte = getchar();

        /*!echo the character*/
        echoCharacter(recvByte);

        /*Backspace handling*/
        if((recvByte == '\b') && (location > 0))
        {
            /*Print a space at the current cursor and move the
             * cursor to previous location
             */
            printf(" ");
            printf("\b");
            location--;
        }
        else
        {
            /*Store the characters for lexical analysis*/
            ipStrPtr[location] = recvByte;
            location++;
        }

    } while((recvByte != '\r') && (location < (size - 1)));

    /*Terminate the string with a null character*/
    ipStrPtr[location] = '\0';

    printf("\n\r");
}

/*
 * This function performs the lexical Analysis of the string
 * entered by the user
 *
 * Parameters: char *input: Pointer to the String
 *
 * Returns: None.
 */
void performLexicalAnalysis(char *input)
{
  char *p = input;
  char *end;
  char *argv[10];
  int argc = 0;
  int characterCount = 0;
  char Commands[5][10];
  bool wordStart = false;
  bool isHandlerInvoked = false;

  /*Find the end of the string*/
  for(end = input; *end != '\0'; end++)
  {
    ;
  }

  memset(argv, 0, sizeof(argv));
  /*Perform Tokenization of the input string*/
  for(p = input; p < end; p++)
  {
    if(*p == ' ' || *p == '\r')
    {
      if(wordStart == true)
      {
        /*If a space or a carriage return is found after the start of the word
         * end the word with a null character
         */
        Commands[argc][characterCount] = '\0';
        argv[argc] = Commands[argc];
        argc++;
        characterCount = 0;
        wordStart = false;
      }
    }
    else
    {
      /*Store the non-space characters*/
      wordStart = true;
      Commands[argc][characterCount] = *p;
      characterCount++;
    }

  }
  argc++;
  argv[argc] = NULL;
  if(argc == 0)
    return;

  /*Invoke the handler as per the input by the user*/
  for(int i = 0; i < numOfCmds; i++)
  {
    /*Case Insensitive comparision of the input string with the command
     * stored in the command processor*/
    if ((strcasecmp(argv[0], commands[i].name)) == 0)
    {
      commands[i].handler(argc ,argv);
      isHandlerInvoked = true;
      break;
    }

  }

  /*Unknown command if no function is invoked*/
  if(isHandlerInvoked == false)
  {
    printf("Unknown Command");
  }

}

/*
 * This function echos the character entered by the user
 *
 * Parameters: char character: Character to be echoed
 *
 * Returns: None.
 */
static void echoCharacter(char character)
{
  printf("%c",character);
}
