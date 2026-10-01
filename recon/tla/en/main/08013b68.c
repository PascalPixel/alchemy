/* Near miss: score 60 with ☀️'s FAKEMATCH form. ⚓️ stores the new count
   after both the value and the address; this form stores it between them.
   Moving the count store later in the source flips the queue and IME
   registers (r1/r4); casts, a global base, a volatile store and 60 s of
   permuting did not fix both. ☀️'s eight sibling writers and
   Input_InitKeyIrq sit beside it in the same order, so one fix ports the
   whole IO module. */
#include "TYPES.H"

struct IoWriteQueue {
    u16 count;
    u16 pad;
    u32 entries[32][3];
};

extern struct IoWriteQueue gIoWriteQueue;

#define REG_IME (*(volatile u16 *)0x04000208)

void QueueIoWriteDelay1(u32 address, u32 value)
{
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    s32 count;

    q = &gIoWriteQueue;
    do {
        ime = &REG_IME;
        saved = *ime;
    } while (0);
    *ime = (u16)ime;
    count = q->count;
    if (count <= 31) {
        u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
        *(u16 *)&q->count = count + 1;
        *destination++ = value;
        *destination++ = address;
        *destination = 0x10000;
    }
    *ime = saved;
}
