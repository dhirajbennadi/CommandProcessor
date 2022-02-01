/*************************************************
 * @filename : main.c
 * @author   : Dhiraj Bennadi
 * @compiler : arm
 * @IDE      : MCUXpresso IDE
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */
#include <stdio.h>
#include "MKL25Z4.h"
#include "sysclock.h"
#include "uart.h"
#include "cli_handler.h"
#include "test_cbfifo.h"

#define LENGTH_OF_CHARS     (256U)

int main(void)
{
    /*Test Cases for Circular Buffer*/
    test_cbfifo();

	int size = LENGTH_OF_CHARS;
	char inputString[LENGTH_OF_CHARS];

	/*Initialize System Clock*/
    sysclock_init();
    /*Initialize UART*/
    InitUart();

    printf("Welcome to BreakfastSerial!");

	while (1)
	{
		printf("\n\r? ");

		accumulateLine(inputString, size);
		performLexicalAnalysis(inputString);
	}

    return 0 ;
}
