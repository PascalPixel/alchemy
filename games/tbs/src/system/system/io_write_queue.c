/// @file
/// The register write queue that VBlank carries out after each finished frame.
// Part of system.c.

#include "../system_internal.h"

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/// The queued register writers: each adds a write of `value` to `address`
/// that the VBlank handler carries out once the frame loop has finished the
/// frame. The control word gives the width and whether the value is stored,
/// ORed in or cleared. The queue holds 32 entries; a write to a full queue is
/// dropped, and a frame that is not finished keeps its writes for the next.
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
/// executor routine on the stack.
void IoWriteQueue_FlushPending(void) {
  struct IoWriteQueue *queue;
  u32 count;

  queue = &gIoWriteQueue;
  count = queue->count;
  if (count != 0) {
    u32 wordCount = (u32)TransferCommandExecutor_Size / 4;
    u8 routineBytes[wordCount * 4];
    QueueFlushRoutine routine = (QueueFlushRoutine)routineBytes;

    // The routine runs from its copy on the stack, which lasts until it ends.
    Dma_Set(Resource_ExecuteTransfer, routine, DMA_COPY_WORDS(wordCount),
            REG_DMA3);
    routine(queue, count);
    queue->count = 0;
  }
}

#undef IO_QUEUE_DEFINE_WRITE
