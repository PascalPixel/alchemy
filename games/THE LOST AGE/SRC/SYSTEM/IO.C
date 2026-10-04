#include "TYPES.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"

/*
 * ☀️'s queued register writes, as ⚓️ builds them: each queues a register
 * write for the next flush with interrupts masked, the entry holding the
 * value, the register address and the delay word, and stores the new count
 * after the value and the address.
 *
 * FAKEMATCH: ☀️'s loop that runs once around the IME read keeps the order of
 * the queue load, the saved copy and the IME store; the IME pointer is held
 * in r1 so that the queue gets r4, as ⚓️ allocates them.
 */
#define DEFINE_QUEUE_IO_WRITE(name, delay)                                  \
    void name(u32 address, u32 value)                                       \
    {                                                                       \
        /* FAKEMATCH: ⚓️ keeps the IME pointer in r1 */                      \
        register volatile u16 *ime asm("r1");                               \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)(u32)ime;                                               \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *destination++ = value;                                         \
            count++;                                                        \
            *destination++ = address;                                       \
            *(u16 *)&q->count = count;                                      \
            *destination = (delay);                                         \
        }                                                                   \
        *ime = saved;                                                       \
    }

/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay1, 0x10000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay2, 0x20000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay3, 0x30000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay5, 0x50000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay6, 0x60000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay7, 0x70000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay9, 0x90000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay10, 0xa0000)
/* FAKEMATCH: the IME pointer is held in r1, as above. */
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay11, 0xb0000)
