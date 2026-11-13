/// @file
/// The register write queue that VBlank carries out after each finished frame.
// Part of system.c.

#include "../system_internal.h"

#define DEFINE_QUEUE_IO_WRITE(name, delay)                                     \
  void name(u32 address, u32 value) {                                          \
    IO_WRITE_QUEUE_PUSH(address, value, delay);                                \
  }

DEFINE_QUEUE_IO_WRITE(IoWriteQueue_Store8, IO_WRITE_8)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_Store16, IO_WRITE_16)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_Store32, IO_WRITE_32)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_SetBits8, IO_WRITE_SET | IO_WRITE_8)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_SetBits16, IO_WRITE_SET | IO_WRITE_16)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_SetBits32, IO_WRITE_SET | IO_WRITE_32)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_ClearBits8, IO_WRITE_CLEAR | IO_WRITE_8)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_ClearBits16, IO_WRITE_CLEAR | IO_WRITE_16)
DEFINE_QUEUE_IO_WRITE(IoWriteQueue_ClearBits32, IO_WRITE_CLEAR | IO_WRITE_32)

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
