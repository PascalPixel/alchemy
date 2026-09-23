#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/* Linker-resolved absolute size of the transfer command executor. */
extern u8 TransferCommandExecutor_Size[];

/*
 * Queue a register write for the next flush with interrupts masked. Each
 * entry holds the value, the register address and the delay word.
 *
 * FAKEMATCH: two odd constructs. The loop that runs once around the IME read
 * is a scheduling barrier that puts the queue literal load first and the
 * saved copy ahead of the IME store. The count is stored through an explicit
 * u16 pointer before the entry is written: the cast gives the store its own
 * alias set, so the scheduler still places it after the value store, while
 * the earlier source position ends the queue pointer's lifetime soon enough
 * for it to be allocated r4 and the IME pointer r1.
 */
#define DEFINE_QUEUE_IO_WRITE(name, delay)                                  \
    void name(u32 address, u32 value)                                       \
    {                                                                       \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
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
            Dma_Set((const void *)0x08002cf4, (void *)routine,
                    word_count | control, (volatile u32 *)0x040000d4);
            routine(queue, count);
            queue->count = zero;
        }
    }
}
