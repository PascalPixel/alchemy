#ifndef GUARD_IO_WRITE_QUEUE_FAMILY_H
#define GUARD_IO_WRITE_QUEUE_FAMILY_H

#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;

#define REG_IME Data_04000208

/*
 * Nonmatching: 2 halfword edits in each of the nine queued-write owners.
 * Allocation, pool order and the rest of the body match; only the queue
 * literal load (ldr r4) comes after the first argument copy instead of
 * before it. The loop that runs once around the saved IME read keeps the
 * saved copy ahead of the IME store, as in the reference. Moving the queue
 * load in front of a barrier on its own gets the order but gives the IME
 * pointer the higher allocation priority, which swaps r1 and r4; re-reading
 * the queue count for the increment fixes the allocation at the cost of an
 * extra load.
 */
#define DEFINE_QUEUE_IO_WRITE(name, delay) \
void name(u32 address, u32 value) \
{ \
    volatile u16 *ime; \
    struct IoWriteQueue *q; \
    u32 saved; \
    s32 count; \
    \
    ime = &REG_IME; \
    q = &gIoWriteQueue; \
    do { \
        saved = *ime; \
    } while (0); \
    *ime = (u16)ime; \
    count = q->count; \
    if (count <= 31) { \
        u32 *destination = (u32 *)((u8 *)q + count * 12 + 4); \
        *destination++ = value; \
        q->count = count + 1; \
        *destination++ = address; \
        *destination = (delay); \
    } \
    *ime = saved; \
}

#endif
