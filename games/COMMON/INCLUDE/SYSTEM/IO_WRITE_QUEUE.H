#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

#include "TYPES.H"

/*
 * Register writes deferred to the next frame. Each entry holds the value, the
 * register address and a delay; the transfer command executor, copied onto
 * the stack, performs the queued writes.
 */
struct IoWriteQueue {
    u16 count;
    u16 pad;
    u32 entries[32][3];
};

extern struct IoWriteQueue gIoWriteQueue;

void QueueIoWriteDelay1(u32 address, u32 value);
void QueueIoWriteDelay2(u32 address, u32 value);
void QueueIoWriteDelay3(u32 address, u32 value);
void QueueIoWriteDelay5(u32 address, u32 value);
void QueueIoWriteDelay6(u32 address, u32 value);
void QueueIoWriteDelay7(u32 address, u32 value);
void QueueIoWriteDelay9(u32 address, u32 value);
void QueueIoWriteDelay10(u32 address, u32 value);
void QueueIoWriteDelay11(u32 address, u32 value);
void IoWriteQueue_FlushPending(void);

/*
 * Queues a register write for the next flush with interrupts masked, written
 * out in place as the QueueIoWriteDelay functions in SYSTEM/IO.C are.
 *
 * FAKEMATCH: the same two odd constructs as IO.C: the one-pass block puts the
 * queue literal load first and the saved copy ahead of the IME store, and the
 * count is stored through its own u16 pointer. A second one-pass block closes
 * after the IME restore, so the stores that follow the push stay behind it
 * (the native code at 080cbc0c restores IME before loading their operands).
 */
#define IO_WRITE_QUEUE_PUSH(address, value, delay)                         \
    {                                                                      \
        volatile u16 *ime;                                                 \
        struct IoWriteQueue *q;                                            \
        u32 saved;                                                         \
        s32 count;                                                         \
                                                                           \
        q = &gIoWriteQueue;                                                \
        do {                                                               \
            do {                                                           \
                ime = (volatile u16 *)0x04000208;                          \
                saved = *ime;                                              \
            } while (0);                                                   \
            *ime = (u16)ime;                                               \
            count = q->count;                                              \
            if (count <= 31) {                                             \
                u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);      \
                *(u16 *)&q->count = count + 1;                             \
                *destination++ = (value);                                  \
                *destination++ = (address);                                \
                *destination = (delay);                                    \
            }                                                              \
            *ime = saved;                                                  \
        } while (0);                                                       \
    }

#endif
