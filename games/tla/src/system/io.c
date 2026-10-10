#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"

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
        /* FAKEMATCH: the repeated IME address ends its first lifetime after the store; without it IME/queue exchange r1/r4 (14 aligned differences per function). CSE removes the assignment. */ \
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
