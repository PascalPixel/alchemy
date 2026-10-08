#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

#include "types.h"

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

#define IO_WRITE_QUEUE_PUSH(address, value, delay)                         \
    {                                                                      \
        /* FAKEMATCH: nested one-pass blocks keep queue/save loads before the IME store, the u16 count store, and following stores after the IME restore. */ \
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
