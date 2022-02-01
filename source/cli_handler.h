/*************************************************
 * @filename : cli_handler.h
 * @author   : Dhiraj Bennadi
 * @compiler : arm
 * @IDE      : MCUXpresso IDE
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */

#ifndef CLI_HANDLER_H_
#define CLI_HANDLER_H_

#include <stdio.h>
#include <stdint.h>

/*
 * This function performs the lexical Analysis of the string
 * entered by the user
 *
 * Parameters: char *input: Pointer to the String
 *
 * Returns: None.
 */
void performLexicalAnalysis(char *input);

/*
 * This function accumulates the characters until a return
 * key is pressed by the user on the terminal window
 *
 * Parameters: char *ipStrPtr: Pointer to the String
 *             int size: Length of the String
 *
 * Returns: None.
 */
void accumulateLine(char *ipStrPtr, int size);

#endif /* CLI_HANDLER_H_ */
