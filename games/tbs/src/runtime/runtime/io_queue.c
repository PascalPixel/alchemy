/// @file
/// The register write queue that VBlank carries out after each finished frame.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/// The queued register writers: each adds a write of `value` to `address`
/// that the VBlank handler carries out once the frame loop has finished the
/// frame. The control word gives the width and whether the value is stored,
/// ORed in or cleared.
#define IO_QUEUE_DEFINE_WRITE(name, control)                                   \
  void name(u32 address, u32 value) {                                          \
    IO_WRITE_QUEUE_PUSH(address, value, control);                              \
  }

/// Queues a byte store.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_Store8, IO_WRITE_8)
/// Queues a halfword store.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_Store16, IO_WRITE_16)
/// Queues a word store.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_Store32, IO_WRITE_32)
/// Queues setting a byte register's bits.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_SetBits8, IO_WRITE_SET | IO_WRITE_8)
/// Queues setting a halfword register's bits.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_SetBits16, IO_WRITE_SET | IO_WRITE_16)
/// Queues setting a word register's bits.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_SetBits32, IO_WRITE_SET | IO_WRITE_32)
/// Queues clearing a byte register's bits.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_ClearBits8, IO_WRITE_CLEAR | IO_WRITE_8)
/// Queues clearing a halfword register's bits.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_ClearBits16, IO_WRITE_CLEAR | IO_WRITE_16)
/// Queues clearing a word register's bits.
IO_QUEUE_DEFINE_WRITE(IoWriteQueue_ClearBits32, IO_WRITE_CLEAR | IO_WRITE_32)

/// Runs the queued register writes and empties the queue, with a copy of the
/// executor routine on the stack. Called by System_VBlankHandler once the
/// frame loop has finished a frame.
void IoWriteQueue_FlushPending(void) {
  struct IoWriteQueue *queue;
  u32 count;

  queue = &gIoWriteQueue;
  count = queue->count;
  if (count != 0) {
    QueueFlushRoutine routine;
    u32 size = (u32)TransferCommandExecutor_Size;
    u32 wordCount = size >> 2;
    u32 zero = 0;
    u32 control = 0x84;

    control <<= 24;
    {
      u8 routineBytes[wordCount << 2];

      // The executor must finish before this stack buffer goes out of scope.
      routine = (QueueFlushRoutine)routineBytes;
      Dma_Set(Resource_ExecuteTransfer, (void *)routine, wordCount | control,
              REG_DMA3);
      routine(queue, count);
      queue->count = zero;
    }
  }
}

#undef IO_QUEUE_DEFINE_WRITE
