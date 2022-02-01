/*
 * test_cbfifo.c - test the cbfifo implementation
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 * 
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "test_cbfifo.h"
#include "cbfifo.h"
#include "common_cbfifo.h"

void test_cbfifo()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  char buf[1024];
  const int cap = cbfifo_capacity();

  // asserts in following 2 lines -- this is not testing the student,
  // it's validating that the test is correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  assert(cap == 255 || cap == 256);

  assert(cbfifo_length(&TestBuffer) == 0);
  assert(cbfifo_dequeue(buf, cap, &TestBuffer) == 0);
  assert(cbfifo_dequeue(buf, 1, &TestBuffer) == 0);

  // enqueue 10 bytes, then dequeue same amt 
  assert(cbfifo_enqueue(str, 10 , &TestBuffer) == 10);
  assert(cbfifo_length(&TestBuffer) == 10);
  assert(cbfifo_dequeue(buf, 10, &TestBuffer) == 10);
  assert(strncmp(buf, str, 10) == 0);
  assert(cbfifo_length(&TestBuffer) == 0);

  // enqueue 20 bytes;  dequeue 5, then another 20
  assert(cbfifo_enqueue(str, 20, &TestBuffer) == 20);
  assert(cbfifo_length(&TestBuffer) == 20);
  assert(cbfifo_dequeue(buf, 5, &TestBuffer) == 5);
  assert(cbfifo_length(&TestBuffer) == 15);
  assert(cbfifo_dequeue(buf+5, 20, &TestBuffer) == 15);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str, 20) == 0);

  // fill buffer and then read it back out
  assert(cbfifo_enqueue(str, cap, &TestBuffer) == cap);
  assert(cbfifo_length(&TestBuffer) == cap);
  assert(cbfifo_enqueue(str, 1, &TestBuffer) == 0);
  assert(cbfifo_dequeue(buf, cap, &TestBuffer) == cap);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // Add 20 bytes and pull out 18
  assert(cbfifo_enqueue(str, 20, &TestBuffer) == 20);
  assert(cbfifo_length(&TestBuffer) == 20);
  assert(cbfifo_dequeue(buf, 18 , &TestBuffer) == 18);
  assert(cbfifo_length(&TestBuffer) == 2);
  assert(strncmp(buf, str, 18) == 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    assert(cbfifo_enqueue(str+i*chunk_size, chunk_size , &TestBuffer) == chunk_size);
    assert(cbfifo_length(&TestBuffer) == (i+1)*chunk_size + 2);
  }
  assert(cbfifo_length(&TestBuffer) == 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  assert(cbfifo_dequeue(buf, 2 , &TestBuffer) == 2);
  assert(strncmp(buf, str+18, 2) == 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    assert(cbfifo_dequeue(buf+i, 1, &TestBuffer) == 1);
    assert(cbfifo_length(&TestBuffer) == chunk_size*4 - i - 1);
  }
  assert(strncmp(buf, str, chunk_size*4) == 0);
  
  // write more than capacity
  assert(cbfifo_enqueue(str, 65, &TestBuffer) == 65);
  assert(cbfifo_enqueue(str+65, cap , &TestBuffer) == cap-65);
  assert(cbfifo_length(&TestBuffer) == cap);
  assert(cbfifo_dequeue(buf, cap, &TestBuffer) == cap);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // write zero bytes
  assert(cbfifo_enqueue(str, 0 , &TestBuffer) == 0);
  assert(cbfifo_length(&TestBuffer) == 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  assert(cbfifo_enqueue(str, 32 , &TestBuffer) == 32);  // advance so that read < write
  assert(cbfifo_length(&TestBuffer) == 32);
  assert(cbfifo_dequeue(buf, 16 , &TestBuffer) == 16);
  assert(cbfifo_length(&TestBuffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, 32 , &TestBuffer) == 32);  // (1)
  assert(cbfifo_length(&TestBuffer) == 48);
  assert(cbfifo_enqueue(str+64, cap-64, &TestBuffer) == cap-64);  // (2)
  assert(cbfifo_length(&TestBuffer) == cap-16);
  assert(cbfifo_dequeue(buf+16, cap-16 , &TestBuffer) == cap-16);
  assert(strncmp(buf, str, cap) == 0);
  
  assert(cbfifo_enqueue(str, 32 , &TestBuffer) == 32);  // advance so that read < write
  assert(cbfifo_length(&TestBuffer) == 32);
  assert(cbfifo_dequeue(buf, 16 , &TestBuffer) == 16);
  assert(cbfifo_length(&TestBuffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap-20 , &TestBuffer) == cap-20);  // (3)
  assert(cbfifo_length(&TestBuffer) == cap-4);
  assert(cbfifo_dequeue(buf, cap-8, &TestBuffer) == cap-8);
  assert(strncmp(buf, str+16, cap-8) == 0);
  assert(cbfifo_length(&TestBuffer) == 4);
  assert(cbfifo_dequeue(buf, 8 , &TestBuffer) == 4);
  assert(strncmp(buf, str+16+cap-8, 4) == 0);
  assert(cbfifo_length(&TestBuffer) == 0);

  assert(cbfifo_enqueue(str, 49 , &TestBuffer) == 49);  // advance so that read < write
  assert(cbfifo_length(&TestBuffer) == 49);
  assert(cbfifo_dequeue(buf, 16, &TestBuffer) == 16);
  assert(cbfifo_length(&TestBuffer) == 33);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+49, cap-33 , &TestBuffer) == cap-33);  // (4)
  assert(cbfifo_length(&TestBuffer) == cap);
  assert(cbfifo_dequeue(buf, cap , &TestBuffer) == cap);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  assert(cbfifo_enqueue(str, 32 , &TestBuffer) == 32);  // advance so that read < write
  assert(cbfifo_length(&TestBuffer) == 32);
  assert(cbfifo_dequeue(buf, 16 , &TestBuffer) == 16);
  assert(cbfifo_length(&TestBuffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap , &TestBuffer) == cap-16);  // (5)
  assert(cbfifo_dequeue(buf, 1 , &TestBuffer) == 1);
  assert(cbfifo_length(&TestBuffer) == cap-1);
  assert(cbfifo_dequeue(buf+1, cap-1 , &TestBuffer) == cap-1);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  assert(cbfifo_enqueue(str, cap-4 , &TestBuffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(&TestBuffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32 , &TestBuffer) == 32);
  rpos += 32;
  assert(cbfifo_length(&TestBuffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12 , &TestBuffer) == 12);
  wpos += 12;
  assert(cbfifo_length(&TestBuffer) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 16 , &TestBuffer) == 16);  // (6)
  assert(cbfifo_length(&TestBuffer) == cap-8);
  assert(cbfifo_dequeue(buf, cap , &TestBuffer) == cap-8);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str+rpos, cap-8) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4 , &TestBuffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(&TestBuffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32, &TestBuffer) == 32);
  rpos += 32;
  assert(cbfifo_length(&TestBuffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12 , &TestBuffer) == 12);
  wpos += 12;
  assert(cbfifo_length(&TestBuffer) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 24 , &TestBuffer) == 24);  // (7)
  assert(cbfifo_length(&TestBuffer) == cap);
  assert(cbfifo_dequeue(buf, cap , &TestBuffer) == cap);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4 , &TestBuffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(&TestBuffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32 , &TestBuffer) == 32);
  rpos += 32;
  assert(cbfifo_length(&TestBuffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12 , &TestBuffer) == 12);
  wpos += 12;
  assert(cbfifo_length(&TestBuffer) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 64 , &TestBuffer) == 24);  // (8)
  assert(cbfifo_length(&TestBuffer) == cap);
  assert(cbfifo_dequeue(buf, cap , &TestBuffer) == cap);
  assert(cbfifo_length(&TestBuffer) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  //printf("%s: passed all test cases\n", __FUNCTION__);
}

