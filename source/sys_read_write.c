/*************************************************
 * @filename : sys_read_write.c
 * @author   : Dhiraj Bennadi
 * @compiler : gcc
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */
#include "uart.h"
#include "cbfifo.h"
#include "MKL25Z4.h"
#include "common_cbfifo.h"

#define ENQUEUE_BYTE_LEN      (1U)
#define DEQUEUE_BYTE_LEN      (1U)
#define ZERO_LEN              (0U)
#define INIT_VAL              (0U)

/*
 * Writes the specified bytes to stdout
 *
 * Parameters: int handle: stdout / stderr
 *             char *buf: Pointer to buffer to print characters
 *             int size: Number of Bytes to print
 *
 * Returns: -1 on error and 0 on success
 */
int __sys_write(int handle, char *buf, int size)
{
	(void)handle;
	char *bufferptr = buf;

	int retVal = size;

	while(size != ZERO_LEN)
	{
		/*wait for space to open up*/
		while(SIZE_OF_CB_BUFFER == cbfifo_length(&TxCbFifo));
		cbfifo_enqueue(bufferptr, ENQUEUE_BYTE_LEN, &TxCbFifo);
		size--;
		bufferptr++;
	}

	/* start transmitter if it isn't already running*/
	if (!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return retVal;
}


/*
 * Reads one character from the serial terminal
 *
 * Parameters: None
 *
 * Returns: -1 on error and character on success
 */
int __sys_readc(void)
{
	int character = INIT_VAL;

	while(ZERO_LEN == cbfifo_length(&RxCbFifo));

	cbfifo_dequeue(&character, DEQUEUE_BYTE_LEN, &RxCbFifo);
	return character;
}
