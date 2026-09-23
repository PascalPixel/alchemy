#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/* Linker-resolved absolute size of the transfer command executor. */
extern u8 TransferCommandExecutor_Size[];

void IoWriteQueue_FlushPending(void)
{
    struct IoWriteQueue *queue;
    u32 count;

    queue = &gIoWriteQueue;
    count = queue->count;
    if (count != 0) {
        QueueFlushRoutine routine;
        u32 size = (u32)TransferCommandExecutor_Size;
        u32 word_count = size >> 2;
        u32 zero = 0;
        u32 control = 0x84;

        control <<= 24;
        {
            u8 routine_bytes[word_count << 2];

            routine = (QueueFlushRoutine)routine_bytes;
            Dma_Set((const void *)0x08002cf4, (void *)routine,
                    word_count | control, (volatile u32 *)0x040000d4);
            routine(queue, count);
            queue->count = zero;
        }
    }
}
