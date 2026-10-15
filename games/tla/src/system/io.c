#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "system.h"
#include "frame.h"
#include "dma.h"

#define DEFINE_QUEUE_IO_WRITE(name, delay)                                  \
    void name(u32 address, u32 value)                                       \
    {                                                                       \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue; /* FAKEMATCH: the one-pass IME read preserves queue-load, saved-copy and IME-store order. */ \
        do {                                                                \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)(u32)ime;                                               \
        /* FAKEMATCH: the repeated IME address ends its first lifetime after the store; without it IME/queue exchange r1/r4 (14 aligned differences per function). CSE removes the assignment. 2026-10-13, tla-ja: written through irqlock.h (IRQ_LOCK and IRQ_UNLOCK, or IME_SAVE, IME_DISABLE and IME_RESTORE, the address set inside the save or before it) the exchange remains (differ 16); on gIoWriteQueue directly 38; the count through a u16 pointer 20. */ \
        ime = &REG_IME;                                                     \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *destination++ = value;                                         \
            count++;                                                        \
            *destination++ = address;                                       \
            q->count = count;                                               \
            *destination = (delay);                                         \
        }                                                                   \
        *ime = saved;                                                       \
    }

#define SOUND_PRESET_COUNT 5

s32 Sound_LoadPresetParameters(s32 preset)
{
    if ((u32)preset > SOUND_PRESET_COUNT - 1)
        preset = 0;
    return 0;
}

void Func_08013b38(void)
{
}

void Func_08013b3c(void)
{
}

static inline void Reg_Write16(u32 value, volatile u16 *reg)
{
    /* FAKEMATCH: through the inline the value stays a word, built by movs, lsls, adds as the native builds 0xc3ff; a halfword constant store loads it from the pool (three rows). */
    *reg = value;
}

void Input_HandleKeyInterrupt(void)
{
    if (gSleepActive == 0) {
        volatile u16 *keycnt = &REG_KEYCNT;

        /* FAKEMATCH: the native loads the register address before it builds the value (one reordered row). */
        __asm__ volatile("" : "+l"(keycnt));
        Reg_Write16(0xc3ff, keycnt);
        gResetRequested = 1;
    }
}

/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay1, 0x10000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay2, 0x20000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay3, 0x30000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay5, 0x50000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay6, 0x60000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay7, 0x70000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay9, 0x90000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay10, 0xa0000)
/* FAKEMATCH: the one-pass read and repeated IME address preserve the native order, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay11, 0xb0000)

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

extern u8 Resource_ExecuteTransferCode[];
extern u8 Resource_ExecuteTransferSize[];

// Runs the queued I/O writes with the transfer routine copied to the stack.
void IoWriteQueue_FlushPending(void)
{
    struct IoWriteQueue *queue;
    u32 count;

    queue = &gIoWriteQueue;
    count = queue->count;
    if (count != 0)
    {
        QueueFlushRoutine routine;
        u32 control = 0x84;

        control <<= 24;
        {
            u8 routine_bytes[(u32)Resource_ExecuteTransferSize];

            routine = (QueueFlushRoutine)routine_bytes;
            Dma_Set(Resource_ExecuteTransferCode, (void *)routine, (sizeof(routine_bytes) >> 2) | control, REG_DMA3);
            routine(queue, count);
            queue->count = 0;
        }
    }
}
