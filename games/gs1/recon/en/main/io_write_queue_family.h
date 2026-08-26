#ifndef GUARD_IO_WRITE_QUEUE_FAMILY_H
#define GUARD_IO_WRITE_QUEUE_FAMILY_H

#include "types.h"

struct IoWriteQueue {
    u16 count;
    u16 pad;
    u32 entries[32][3];
};

extern struct IoWriteQueue Data_02002090;
extern volatile u16 Data_04000208;

#define gIoWriteQueue Data_02002090
#define REG_IME Data_04000208

#define DEFINE_QUEUE_IO_WRITE(name, delay) \
void name(u32 address, u32 value) \
{ \
    volatile u16 *ime = &REG_IME; \
    struct IoWriteQueue *q = &gIoWriteQueue; \
    do { \
        u32 saved; \
        \
        saved = *ime; \
        *ime = (u16)ime; \
        do { \
            s32 cnt; \
            \
            cnt = q->count; \
            if (cnt <= 31) { \
                u32 *dst = (u32 *)((u8 *)q + cnt * 12 + 4); \
                *dst++ = value; \
                q->count = cnt + 1; \
                *dst++ = address; \
                *dst = (delay); \
            } \
        } while (0); \
        *ime = saved; \
    } while (0); \
}

#endif
