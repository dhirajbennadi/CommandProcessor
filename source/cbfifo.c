/*************************************************
 * @filename : cbfifo.c
 * @author   : Dhiraj Bennadi
 * @compiler : gcc
 *
 * Identification of any leveraged code : None
 * Applicable Links: None
 */

/*Standard Library Inclusion*/
#include <stdlib.h>
#include <stdint.h>  // for size_t
#include <stdbool.h>
/*User Defined Files Inclusion*/
#include "cbfifo.h"
#include "common_cbfifo.h"
#include "MKL25Z4.h"

/*Circular Buffer Object*/
cbfifo_t RxCbFifo = {{0},0,0,0};
cbfifo_t TxCbFifo = {{0},0,0,0};
cbfifo_t TestBuffer = {{0},0,0,0};

/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 */
size_t cbfifo_enqueue(void *buf, size_t nbyte, cbfifo_t *cbBuffer)
{

  /*Error Case if the enqueing buffer is NULL*/
  if((buf == NULL))
  {
    return -1;
  }

  uint32_t masking_state = __get_PRIMASK();
  __disable_irq();
  uint32_t enqueue_count = 0;
  /*Number of bytes remaining in the CB Fifo to enqueue data*/
  enqueue_count = SIZE_OF_CB_BUFFER - cbBuffer->length; //cbfifoObj.length;
  /*Number of bytes to enqueue*/
  if(enqueue_count >= nbyte)
  {
    enqueue_count = nbyte;
  }


  for(uint8_t index = 0; index < enqueue_count; index++)
  {
    *(cbBuffer->cbfifo + cbBuffer->write ) = *(uint8_t*)buf;
    /*Roll over to the first location when the capacity of the buffer is reached*/
    cbBuffer->write = (cbBuffer->write + 1) % SIZE_OF_CB_BUFFER;
    (uint8_t*)buf++;

    cbBuffer->length++;
  }


  __set_PRIMASK(masking_state);

  /*Count of the number of bytes actually enqueued in the CB FIFO*/
  return enqueue_count;

}


/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(void *buf, size_t nbyte, cbfifo_t *cbBuffer)
{

  /*Error Case if the dequeing buffer is NULL*/
  if((buf == NULL))
  {
    return -1;
  }

  uint32_t masking_state = __get_PRIMASK();
  __disable_irq();
  uint8_t dequeue_count = 0;
  /*If the nbyte is greater than the length of the buffer, the number of
   * bytes to dequeue equals the length of buffer*/
  if(cbBuffer->length <= nbyte)
  {
    dequeue_count = cbBuffer->length;
  }
  else
  {
    dequeue_count = nbyte;
  }

  for(uint8_t index = 0; index < dequeue_count; index++)
  {
    *(uint8_t*)buf = *(cbBuffer->cbfifo + cbBuffer->read);
    //*(cbBuffer->cbfifo + cbBuffer->read) = 0;
    (uint8_t*)buf++;
    /*Roll over to the first location when the capacity of the buffer is reached*/
    cbBuffer->read = (cbBuffer->read + 1) % SIZE_OF_CB_BUFFER;

    cbBuffer->length--;
  }



  __set_PRIMASK(masking_state);
  /*Count of the number of bytes dequeued from the CB FIFO*/
  return dequeue_count;
}

/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(cbfifo_t *cbBuffer)
{
  return cbBuffer->length;
}

/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
size_t cbfifo_capacity()
{
  return SIZE_OF_CB_BUFFER;

}
