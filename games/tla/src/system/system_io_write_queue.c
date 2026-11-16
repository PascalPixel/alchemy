/// @file
/// The register write queue that VBlank carries out after each finished frame.
// Part of system.c.

#include "system_internal.h"

/// The queued register writers: each adds a write of `value` to `address` that
/// the VBlank handler carries out once the frame loop has finished the frame.
#define DEFINE_QUEUE_IO_WRITE(name, delay)                                     \
  void name(u32 address, u32 value) {                                          \
    IO_WRITE_QUEUE_PUSH(address, value, delay);                                \
  }

/// Queues a byte store.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_Store8, IO_WRITE_BYTE)
/// Queues a halfword store.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_Store16, IO_WRITE_HALFWORD)
/// Queues a word store.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_Store32, IO_WRITE_WORD)
/// Queues setting a byte register's bits.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_SetBits8, IO_WRITE_SET | IO_WRITE_BYTE)
/// Queues setting a halfword register's bits.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_SetBits16, IO_WRITE_SET | IO_WRITE_HALFWORD)
/// Queues setting a word register's bits.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_SetBits32, IO_WRITE_SET | IO_WRITE_WORD)
/// Queues clearing a byte register's bits.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_ClearBits8, IO_WRITE_CLEAR | IO_WRITE_BYTE)
/// Queues clearing a halfword register's bits.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_ClearBits16,
                      IO_WRITE_CLEAR | IO_WRITE_HALFWORD)
/// Queues clearing a word register's bits.
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_ClearBits32, IO_WRITE_CLEAR | IO_WRITE_WORD)

/// Runs the queued I/O writes with the transfer routine copied to the stack.
void IoWriteQueue_FlushPending(void) {
  struct IoWriteQueue *queue;
  u32 count;

  queue = &gIoWriteQueue;
  count = queue->count;
  if (count != 0) {
    QueueFlushRoutine routine;
    u32 control = (DMA_ENABLE | DMA_32BIT) << 16;

    {
      u8 routineBytes[(u32)Resource_ExecuteTransferSize];

      routine = (QueueFlushRoutine)routineBytes;
      Dma_Set(Resource_ExecuteTransferCode, (void *)routine,
              (sizeof(routineBytes) >> 2) | control, REG_DMA3);
      routine(queue, count);
      queue->count = 0;
    }
  }
}

#undef DEFINE_QUEUE_IO_WRITE
