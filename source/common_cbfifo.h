/*************************************************
 * @filename : common_cbfifo.h
 * @author   : Dhiraj Bennadi
 * @compiler : gcc
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */

#ifndef _COMMON_CBFIFO_H_
#define _COMMON_CBFIFO_H_

#include <stdint.h>

#define SIZE_OF_CB_BUFFER 255


typedef struct cbfifo_s {
  uint8_t cbfifo[SIZE_OF_CB_BUFFER];
  uint8_t read;
  uint8_t write;
  uint8_t length;
  }cbfifo_t;


/*Circular Buffer for Input Stream*/
extern cbfifo_t RxCbFifo;
/*Circular Buffer for Output Stream*/
extern cbfifo_t TxCbFifo;
/*Circular Buffer for Testing the cbfifo test cases*/
extern cbfifo_t TestBuffer;


#endif // _COMMON_CBFIFO_H_
