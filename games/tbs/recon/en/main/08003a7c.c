#include "io_write_queue_family.h"
#include "video_dma_family.h"

#define IoWriteQueue_FlushPending Func_08003a7c

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/* Linker-resolved absolute size: 0x68. */
extern u8 TransferCommandExecutor_Size[];

void IoWriteQueue_FlushPending(void)
{
    struct IoWriteQueue *queue;
    u32 count;

    queue = &gIoWriteQueue;
    count = queue->count;
    if (count != 0) {
        const void *dma_source;
        void *dma_destination;
        u32 dma_control;
        QueueFlushRoutine routine;
        u32 size = (u32)TransferCommandExecutor_Size;
        u32 dma_word_count = size >> 2;
        u32 zero = 0;
        u32 dma_control_base = 0x84;

        dma_control_base <<= 24;

        {
            u8 routine_bytes[dma_word_count << 2];

            routine = (QueueFlushRoutine)routine_bytes;
            dma_source = (const void *)0x08002cf4;
            dma_destination = (void *)routine;
            dma_control = dma_word_count | dma_control_base;
            StartDmaTransfer(dma_source, dma_destination, dma_control);
            routine(queue, count);
            queue->count = zero;
            /*
             * Extent probe: the scalar DMA spelling is four bytes shorter
             * than the retained movmem-shaped owner. Keep the complete queue
             * header access explicit until that store shape is recovered.
             */
            *(volatile u16 *)&queue->pad = *(volatile u16 *)&queue->pad;
        }
    }
}
