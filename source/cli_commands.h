/*************************************************
 * @filename : cli_commands.h
 * @author   : Dhiraj Bennadi
 * @compiler : arm
 * @IDE      : MCUXpresso IDE
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */

#ifndef CLI_COMMANDS_H_
#define CLI_COMMANDS_H_

#include <stdio.h>

/*Function Pointer to Invoke Commands by the input from the user*/
typedef void (*command_handler_t)(int argc, char *argv[]);

/*Structure to store the parameters of the commands*/
typedef struct
{
    const char* name;
    command_handler_t handler;
    const char* helpString;
}command_table_t;

/* Object of the command_table_t*/
extern const command_table_t commands[];

/* Number of Commands currently supported in the command processor*/
extern const int numOfCmds;


#endif /* CLI_COMMANDS_H_ */
