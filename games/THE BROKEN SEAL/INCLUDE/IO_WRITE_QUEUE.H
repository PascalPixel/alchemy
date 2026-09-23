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

extern struct IoWriteQueue Data_02002090;
#define gIoWriteQueue Data_02002090

void IoWriteQueue_FlushPending(void);

#endif
