#include "GLOBAL.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "FRAME.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"

void Input_HandleKeyInterrupt(void)
{
    u32 mask;

    if (gSleepActive == 0) {
        /* FAKEMATCH: the word mask loads before KEYCNT; a halfword constant reverses those loads and literal order. */
        mask = 0xc3ff;
        REG_KEYCNT = mask;
        gResetRequested = 1;
    }
}


typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);


#define DEFINE_QUEUE_IO_WRITE(name, delay)                                  \
    void name(u32 address, u32 value)                                       \
    {                                                                       \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue; /* FAKEMATCH: the one-pass IME read and early u16 count store preserve queue-first loads and r4/r1 pointer allocation. */                                                 \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = value;                                         \
            *destination++ = address;                                       \
            *destination = (delay);                                         \
        }                                                                   \
        *ime = saved;                                                       \
    }

DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay1, 0x10000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay2, 0x20000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay3, 0x30000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay5, 0x50000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay6, 0x60000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay7, 0x70000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay9, 0x90000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay10, 0xa0000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay11, 0xb0000)

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
            Dma_Set((const void *)Resource_ExecuteTransfer, (void *)routine,
                    word_count | control, REG_DMA3);
            routine(queue, count);
            queue->count = zero;
        }
    }
}
