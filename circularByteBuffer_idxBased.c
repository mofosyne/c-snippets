// # Circular Byte Buffer For Embedded Applications (Index Based)
// Author: Brian Khuu (July 2020) (briankhuu.com) (mofosyne@gmail.com)
// Reason: Malloc free, minimum overhead implementation of a circular buffer.
//   Static inlined handlers for speed and ease of usage in various projects.
//   Index based implementation diverges from harshkn's version, since it is
//   easier for me to grok. However may come at cost of speed and optimisation.
//   Also uses byte based rather than item based for easier understability
//   when used for simpler byte array circular buffers.
//   I have also written an pointer based circular buffer version.
// Based on harshkn's circular buffer: https://gist.github.com/harshkn/909546
// This Gist (Pointer): https://gist.github.com/mofosyne/d7a4a8d6a567133561c18aaddfd82e6f
// This Gist (Index): https://gist.github.com/mofosyne/82020d5c0e1e11af0eb9b05c73734956

#include <stdint.h> // uint8_t
#include <stddef.h> // size_t
#include <stdbool.h> // bool

// Prefill Circular Buffer (Allows for skipping `circularBuffer_uint8_Init()`)
#define circularBuffer_uint8_struct_full_prefill(BuffSize, BuffPtr) \
{                                                                   \
  .capacity  = BuffSize,                                            \
  .count     = 0,                                                   \
  .buffer    = BuffPtr,                                             \
  .head      = 0,                                                   \
  .tail      = 0                                                    \
}
#define circularBuffer_uint8_struct_prefill(Buff) circularBuffer_uint8_struct_full_prefill((sizeof(Buff)/sizeof(Buff[0])), &Buff[0])

typedef struct circularBuffer_uint8_t
{
  size_t capacity; ///< Maximum number of items in the buffer
  size_t count;    ///< Number of items in the buffer
  uint8_t *buffer; ///< Data Buffer
  size_t head;     ///< Head Index
  size_t tail;     ///< Tail Index
} circularBuffer_uint8_t;


/*******************************************************************************
 * Init/IsInit/Reset
*******************************************************************************/

static inline bool circularBuffer_uint8_Init(circularBuffer_uint8_t *cb, size_t capacity, uint8_t *buffPtr)
{
  circularBuffer_uint8_t emptyCB = {0};
  if ((cb == NULL) || (buffPtr == NULL))
    return false; ///< Failed
  // Init Struct
  *cb = emptyCB;
  cb->capacity = capacity;
  cb->buffer = buffPtr;
  cb->count = 0;
  cb->head = 0;
  cb->tail = 0;
  return true; ///< Successful
}

static inline bool circularBuffer_uint8_IsInit(circularBuffer_uint8_t *cb)
{
  return cb->capacity && cb->buffer;
}

static inline bool circularBuffer_uint8_Reset(circularBuffer_uint8_t *cb)
{
  cb->count = 0;
  cb->head = 0;
  cb->tail = 0;
  return true; ///< Successful
}


/*******************************************************************************
 * Circular byte buffer Enqueue/Dequeue (This will modify the buffer)
*******************************************************************************/

static inline bool circularBuffer_uint8_EnqueueOverwrite(circularBuffer_uint8_t *cb, const uint8_t b)
{
  if (cb->count >= cb->capacity)
  {
    // Full. Increment head
    cb->head = (cb->head + 1) % cb->capacity;
  }
  else
  {
    // Not Full. Update Counter
    cb->count = cb->count + 1;
  }
  // Push value
  cb->buffer[cb->tail] = b;
  // Increment tail
  cb->tail = (cb->tail + 1) % cb->capacity;
  return true; ///< Successful
}

static inline bool circularBuffer_uint8_Enqueue(circularBuffer_uint8_t *cb, const uint8_t b)
{
  // Full
  if (cb->count >= cb->capacity)
    return false; ///< Failed
  // Push value
  cb->buffer[cb->tail] = b;
  // Increment tail
  cb->tail = (cb->tail + 1) % cb->capacity;
  cb->count = cb->count + 1;
  return true; ///< Successful
}

static inline bool circularBuffer_uint8_Dequeue(circularBuffer_uint8_t *cb, uint8_t *b)
{
  // Empty
  if (cb->count == 0)
    return false; ///< Failed
  // Pop value
  *b = cb->buffer[cb->head];
  // Increment head
  cb->head = (cb->head + 1) % cb->capacity;
  cb->count = cb->count - 1;
  return true; ///< Successful
}


/*******************************************************************************
 * Circular byte buffer Peek (Will Not Modify Buffer)
*******************************************************************************/

static inline bool circularBuffer_uint8_Peek(circularBuffer_uint8_t *cb, uint8_t *b, const size_t offset)
{
  // Empty?
  if (cb->count == 0)
    return false; ///< Failed
  if (cb->count < offset)
    return false; ///< Failed
  *b = cb->buffer[(cb->head + offset) % cb->capacity];
  return true; ///< Successful
}


/*******************************************************************************
 * Circular byte buffer utility functions (Will Not Modify Buffer)
*******************************************************************************/

static inline size_t circularBuffer_uint8_Capacity(circularBuffer_uint8_t *cb)
{
  return cb->capacity;
}

static inline size_t circularBuffer_uint8_Count(circularBuffer_uint8_t *cb)
{
  return cb->count;
}

static inline bool circularBuffer_uint8_IsFull(circularBuffer_uint8_t *cb)
{
  return (cb->count >= cb->capacity);
}

static inline bool circularBuffer_uint8_IsEmpty(circularBuffer_uint8_t *cb)
{
  return (cb->count == 0);
}


#if 1 // Last Confirmed Working On 2021-07-07 By Brian Khuu mofosyne@gmail.com
/*******************************************************************************
 * Mini Unit Test Of Circular Buffer
*******************************************************************************/
#include <stdio.h>
#define BUFF_TEST_SIZE 4

// Minimum Assert Unit (https://jera.com/techinfo/jtns/jtn002)
#define mu_assert(message, test) do { if (!(test)) return LINEINFO " : (expect:" #test ") " message; } while (0)
#define mu_run_test(test) do { char *message = test(); if (message) return message; } while (0)

// Line Info (Ref: __LINE__ to string http://decompile.com/cpp/faq/file_and_line_error_string.htm)
#define LINEINFO_STR(X) #X
#define LINEINFO__STR(X) LINEINFO_STR(X)
#define LINEINFO __FILE__ " : " LINEINFO__STR(__LINE__)

#if 0 // Dev Note: Use this to debug the content of the circular buffer
void circularBuffer_inspect(circularBuffer_uint8_t *cb, char * msg)
{
  printf("\n%s : ", msg);
  for (int i = 0 ; i < cb->capacity ; i++)
    printf(" %d", cb->buffer[i]);
  printf("\n");
}
#endif

char * cbuff_test_prefill(void)
{
  uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
  circularBuffer_uint8_t prefilledBuff = circularBuffer_uint8_struct_prefill(cbuffer);
  circularBuffer_uint8_t initBuff = {0};
  mu_assert("", !circularBuffer_uint8_IsInit(&initBuff));
  circularBuffer_uint8_Init(&initBuff, BUFF_TEST_SIZE, cbuffer);
  mu_assert("", circularBuffer_uint8_IsInit(&initBuff));
  mu_assert("", circularBuffer_uint8_Capacity(&prefilledBuff) == BUFF_TEST_SIZE);
  mu_assert("", circularBuffer_uint8_Count(&prefilledBuff) == 0);
  mu_assert("", !circularBuffer_uint8_IsFull(&prefilledBuff));
  mu_assert("", circularBuffer_uint8_IsEmpty(&prefilledBuff));
  mu_assert("", prefilledBuff.capacity == initBuff.capacity );
  mu_assert("", prefilledBuff.count    == initBuff.count    );
  mu_assert("", prefilledBuff.buffer   == initBuff.buffer   );
  mu_assert("", prefilledBuff.head     == initBuff.head     );
  mu_assert("", prefilledBuff.tail     == initBuff.tail     );
  circularBuffer_uint8_Reset(&initBuff);
  mu_assert("", prefilledBuff.count    == initBuff.count    );
  mu_assert("", prefilledBuff.head     == initBuff.head     );
  mu_assert("", prefilledBuff.tail     == initBuff.tail     );
  return 0;
}

char * cbuff_test_general(void)
{
  uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
  circularBuffer_uint8_t prefilledBuff = circularBuffer_uint8_struct_prefill(cbuffer);
  for (int i = 0 ; i < BUFF_TEST_SIZE ; i++)
  {
    mu_assert("", !circularBuffer_uint8_IsFull(&prefilledBuff));
    mu_assert("", circularBuffer_uint8_Enqueue(&prefilledBuff, i));
    mu_assert("", !circularBuffer_uint8_IsEmpty(&prefilledBuff));
    mu_assert("", circularBuffer_uint8_Count(&prefilledBuff) == (i+1));
  }
  mu_assert("", !circularBuffer_uint8_Enqueue(&prefilledBuff, 0x33));
  mu_assert("", circularBuffer_uint8_IsFull(&prefilledBuff));
  mu_assert("", !circularBuffer_uint8_IsEmpty(&prefilledBuff));
  for (int i = 0 ; i < BUFF_TEST_SIZE ; i++)
  {
    uint8_t d = -1;
    mu_assert("", circularBuffer_uint8_Dequeue(&prefilledBuff, &d));
    mu_assert("", d == i);
  }
  return 0;
}

char * cbuff_test_overwrite(void)
{
  uint8_t cbuffer[BUFF_TEST_SIZE] = {0};
  circularBuffer_uint8_t prefilledBuff = circularBuffer_uint8_struct_prefill(cbuffer);
  for (int i = 0 ; i < BUFF_TEST_SIZE ; i++)
  {
    circularBuffer_uint8_Enqueue(&prefilledBuff, i);
  }
  for (int i = 0 ; i < BUFF_TEST_SIZE ; i++)
  {
    circularBuffer_uint8_EnqueueOverwrite(&prefilledBuff, i+1);
  }
  for (int i = 0 ; i < BUFF_TEST_SIZE ; i++)
  {
    uint8_t d = -1;
    mu_assert("", circularBuffer_uint8_Dequeue(&prefilledBuff, &d));
    mu_assert("", d == i + 1);
  }
  return 0;
}

char * cbuff_test_peek(void)
{
  uint8_t cbuffer[3] = {0};
  circularBuffer_uint8_t prefilledBuff = circularBuffer_uint8_struct_prefill(cbuffer);
  for (int i = 1 ; i < 5 ; i++)
  {
    circularBuffer_uint8_EnqueueOverwrite(&prefilledBuff, i);
  }
  for (int i = 0 ; i < 3 ; i++)
  {
    uint8_t d = -1;
    mu_assert("", circularBuffer_uint8_Peek(&prefilledBuff, &d, i));
    mu_assert("", d == i+2);
  }
  return 0;
}

static char * all_tests()
{
  mu_run_test(cbuff_test_prefill);
  mu_run_test(cbuff_test_general);
  mu_run_test(cbuff_test_overwrite);
  mu_run_test(cbuff_test_peek);
  return 0;
}

int main(void)
{
  char *result = all_tests();
  printf("%s\n", (result) ? result : "ALL TESTS PASSED\n");
  return result != 0;
}
#endif