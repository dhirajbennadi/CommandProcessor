/*************************************************
 * @filename : cli_commands.c
 * @author   : Dhiraj Bennadi
 * @compiler : arm
 * @IDE      : MCUXpresso IDE
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */

#include <string.h>
#include "cli_commands.h"
#include "hexdump.h"
#include <stdlib.h>

#define MAX_BYTES_HEXDUMP       (640)
#define BASE_10                 (10)
#define BASE_16                 (16)
#define MIN_ARGUMENTS_HEXDUMP   (4U)

static void handleAuthor(int argc, char *argv[]);
static void handleHelp(int argc, char* argv[]);
static void handleDump(int argc, char *argv[]);


const command_table_t commands[] =
{
        {"author", handleAuthor, "Prints the name of author"},
        {"dump", handleDump, " <loc> <len> \r\n\t Prints hexdump of RAM from address loc\r\n"\
                                         "\t The address loc is in hex (""0xNN""); Length is in"\
                                         " hex(""0xNN"") or dec (""NN"")"},
        {"help", handleHelp, "Prints list of commands available to the user"}
};

const int numOfCmds = sizeof(commands) / sizeof(command_table_t);

/*
 * Handler for the command author (case insensitive)
 *
 * Parameters: int argc: Argument Count
 *             char *argv: Pointer to the array of arguments
 *
 * Returns: None.
 */
static void handleAuthor(int argc, char *argv[])
{
    printf("Dhiraj Bennadi");
}

/*
 * Handler for the command help (case insensitive)
 *
 * Parameters: int argc: Argument Count
 *             char *argv: Pointer to the array of arguments
 *
 * Returns: None.
 */
static void handleHelp(int argc, char* argv[])
{
    for(int i = 0; i < numOfCmds; i++)
    {
        printf("%s \n", commands[i].name);
        printf("%s \r\n", commands[i].helpString);
    }
}

/*
 * Handler for the command dump (case insensitive)
 *
 * Parameters: int argc: Argument Count
 *             char *argv: Pointer to the array of arguments
 * Returns: None.
 */
static void handleDump(int argc, char *argv[])
{
    char *ptr;
    long startAddr;
    long numBytes;

    if(argc < MIN_ARGUMENTS_HEXDUMP)
    {
      printf("Invalid Command\n\r");
      return;
    }

    /*!
     * Start Address of Hexdump
     */
    startAddr = strtol(argv[1], &ptr, BASE_16);

    /*
     * Number of Bytes to be printed
     */
    if((argv[2][0] == '0') && (argv[2][1] == 'x'))
    {
      /*Number of Bytes if the user input is in Hex Values*/
      numBytes = strtol(argv[2], &ptr, BASE_16);
    }
    else
    {
      /*Number of Bytes if the user input is in Decimal Values*/
      numBytes = strtol(argv[2], &ptr, BASE_10);
    }

    if(numBytes > MAX_BYTES_HEXDUMP)
    {
      printf("Invalid Address\n\r");
      return;
    }

    hexdump(startAddr, numBytes);
}
