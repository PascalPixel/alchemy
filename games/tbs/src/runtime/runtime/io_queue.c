// Part of runtime.c, the runtime object, compiled only through it.
// The register write queue that VBlank carries out frames later.

#include "../runtime_internal.h"

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

// The queued register writers: each adds a write of `value` to `address` that
// the VBlank handler carries out `delay` frames later (the delay sits in the
// high half of the entry's control word).
#define IO_QUEUE_DEFINE_WRITE(name, delay)                                     \
  void name(u32 address, u32 value) {                                          \
    IO_WRITE_QUEUE_PUSH(address, value, delay);                                \
  }

// Queues a register write for the next VBlank.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay1, 1 << 16)
// Queues a register write two frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay2, 2 << 16)
// Queues a register write three frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay3, 3 << 16)
// Queues a register write five frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay5, 5 << 16)
// Queues a register write six frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay6, 6 << 16)
// Queues a register write seven frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay7, 7 << 16)
// Queues a register write nine frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay9, 9 << 16)
// Queues a register write ten frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay10, 10 << 16)
// Queues a register write eleven frames ahead.
IO_QUEUE_DEFINE_WRITE(QueueIoWriteDelay11, 11 << 16)

// Called from VBlank: runs the queued register writes, with a copy of the
// executor routine on the stack.
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
