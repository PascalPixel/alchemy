/// @file
/// The register write queue that VBlank carries out after each finished frame.
// Part of system.c.

#include "../system_internal.h"

#define DEFINE_QUEUE_IO_WRITE(name, delay)                                     \
  void name(u32 address, u32 value) {                                          \
    volatile u16 *ime;                                                         \
    struct IoWriteQueue *q;                                                    \
    u32 saved;                                                                 \
    s32 count;                                                                 \
                                                                               \
    /* FAKEMATCH: the one-pass IME read keeps the queue load, saved copy and   \
     * IME store in the native order. */                                       \
    q = &gIoWriteQueue;                                                        \
    do {                                                                       \
      /* FAKEMATCH: the one-pass block is a barrier for the IME read. */       \
      ime = &REG_IME;                                                          \
      saved = *ime;                                                            \
    } while (0);                                                               \
    *ime = (u16)(u32)ime;                                                      \
    /* FAKEMATCH: the repeated IME address ends its first lifetime after the   \
     * store, so IME and the queue keep r1 and r4; without it, 14              \
     * instructions differ per function. */                                    \
    ime = &REG_IME;                                                            \
    count = q->count;                                                          \
    if (count <= 31) {                                                         \
      u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);                    \
      *destination++ = value;                                                  \
      count++;                                                                 \
      *destination++ = address;                                                \
      q->count = count;                                                        \
      *destination = (delay);                                                  \
    }                                                                          \
    *ime = saved;                                                              \
  }

/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay1, IO_WRITE_8)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay2, IO_WRITE_16)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay3, IO_WRITE_32)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay5, IO_WRITE_SET | IO_WRITE_8)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay6, IO_WRITE_SET | IO_WRITE_16)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay7, IO_WRITE_SET | IO_WRITE_32)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay9, IO_WRITE_CLEAR | IO_WRITE_8)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay10, IO_WRITE_CLEAR | IO_WRITE_16)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native
 * order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay11, IO_WRITE_CLEAR | IO_WRITE_32)

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

extern u8 Resource_ExecuteTransferCode[];
extern u8 Resource_ExecuteTransferSize[];

/// Runs the queued I/O writes with the transfer routine copied to the stack.
void IoWriteQueue_FlushPending(void) {
  struct IoWriteQueue *queue;
  u32 count;

  queue = &gIoWriteQueue;
  count = queue->count;
  if (count != 0) {
    QueueFlushRoutine routine;
    u32 control = 0x84;

    control <<= 24;
    {
      u8 routine_bytes[(u32)Resource_ExecuteTransferSize];

      routine = (QueueFlushRoutine)routine_bytes;
      Dma_Set(Resource_ExecuteTransferCode, (void *)routine,
              (sizeof(routine_bytes) >> 2) | control, REG_DMA3);
      routine(queue, count);
      queue->count = 0;
    }
  }
}

#undef DEFINE_QUEUE_IO_WRITE
