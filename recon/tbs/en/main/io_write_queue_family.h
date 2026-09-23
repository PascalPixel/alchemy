#ifndef GUARD_IO_WRITE_QUEUE_FAMILY_H
#define GUARD_IO_WRITE_QUEUE_FAMILY_H

#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;

#define REG_IME Data_04000208

#define DEFINE_QUEUE_IO_WRITE(name, delay) \
void name(u32 address, u32 value) \
{ \
    volatile u16 *ime = &REG_IME; \
    struct IoWriteQueue *q = &gIoWriteQueue; \
    u32 saved; \
    s32 count; \
    \
    saved = *ime; \
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
