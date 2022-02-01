/*************************************************
 * @filename : hexdump.c
 * @author   : Dhiraj Bennadi
 * @compiler : arm
 * @IDE      : MCUXpresso IDE
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */

#include "hexdump.h"
#include <stdio.h>

#define STRIDE    (16U)

/*
 * This function prints the dump of the memory
 *
 * Parameters: long address: Start Address
 *             long length: Number of Bytes
 *
 * Returns: None.
 */
void hexdump(long address, long length)
{
    long index = 0;
    char *ptr = address;

    for(index = 0; index < length; index++)
    {
        if(0 == (index % STRIDE))
        {
          if(index == 0)
          {
            printf("%08X ", ptr);
          }
          else
          {
            printf("\r\n%08X ", ptr);
          }
        }

        printf("%02X ",*ptr);
        ptr++;
    }
}
