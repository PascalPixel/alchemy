/* Contiguous unnamed state-owner run for resource_383. */

#include "types.h"

/*
 * Update one actor's animation descriptor when its current state matches the
 * expected state. The helper is called by the 17-entry scene transition table
 * at 0x02002564.
 *
 * The owner starts with push {r5,r6,r7,lr} at 0x020026e4, returns through
 * pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02002716-0x0200271a, and is immediately
 * followed by the callback owner at 0x0200271c. It has no trailing pool, so
 * the complete span is 56 bytes.
 */

extern u8 *Func_02007472(s32 actor);
extern void Func_02007496(s32 actor, const u8 *descriptor);

void Func_020026e4(s32 actor, s32 expected, s32 next, const u8 *descriptor)
{
    u8 *record = Func_02007472(actor);

    if (*(s16 *)(record + 100) == expected) {
        Func_02007496(actor, descriptor);
        *(u16 *)(record + 100) = (u16)next;
    }
}

#include "types.h"

extern u8 *Func_0200749e(s32 actor);
extern u8 *Func_020074a6(s32 actor);
extern void Func_02007462(s32 flag);
extern void Func_02007474(s32 flag);

void Func_0200271c(void)
{
    u8 *actor0 = Func_0200749e(0);
    s32 raw_x = *(s32 *)(actor0 + 8);
    s32 x;
    s32 z;

    u8 *actor1 = Func_020074a6(0);
    x = raw_x >> 20;
    z = *(s32 *)(actor1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        Func_02007462(148 << 2);
    } else {
        Func_02007474(148 << 2);
    }
}

