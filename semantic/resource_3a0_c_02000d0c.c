#include "types.h"

/*
 * Resource 3a0 owner at 0x02000d0c (220 bytes).
 *
 * Complete owner: r8-saving prologue and 12-byte target frame at 0x02000d0c,
 * through the sole unwind return at 0x02000dd8-0x02000de2, followed by the
 * referenced 0xfff00000 mask word at 0x02000de4.  The next owner begins at
 * 0x02000de8.
 *
 * If story flag 0x200 is set, the routine centers a target on actor 0's tile,
 * displaces it by the actor's quadrant heading, and collision-checks it.  An
 * unobstructed target runs a bracketed animation/state sequence, clears bits
 * 0 and 7 of status byte +0x55 while moving, reports the target's signed
 * integer coordinates, then restores the byte exactly.
 *
 * Both the absent-flag and blocked-target paths return before opening the
 * scene bracket, matching the two direct branches to the epilogue.  All
 * fourteen calls resolve across eleven veneer targets and match below in
 * multiset and source/machine order.
 */













extern u8 * Func_020020e8();
extern s32 Func_020020ca();
extern void Func_020020ba();
extern s32 Func_020020f2();
extern void Func_02002132();
extern void Func_020020da();
extern void Func_020020b8();
extern void Func_0200223e();
extern void Func_020020ee();
extern void Func_02002140();
extern void Func_020021b6();
extern void Func_02002126();
extern void Func_0200215e();
extern void Func_0200219e();
void Func_02000d0c(void)
{
    u8 *actor = Func_020020e8(0);
    u8 savedStatus = actor[0x55];
    s32 target[3];
    u16 heading;

    if (Func_020020ca(0x200) == 0) {
        return;
    }

    target[0] = (*(s32 *)(actor + 8) & (s32)0xfff00000) + 0x80000;
    target[1] = *(s32 *)(actor + 12);
    target[2] = (*(s32 *)(actor + 16) & (s32)0xfff00000) + 0x80000;

    heading = (u16)((*(u16 *)(actor + 6) + 0x2000) & 0xc000);
    Func_020020ba(0x200000, heading, target);

    if (Func_020020f2(actor, target) != 0) {
        return;
    }

    Func_02002132();
    Func_020020da(actor, 6);
    Func_020020b8(6);
    Func_0200223e(152);
    Func_020020ee(actor, 7);

    *(s32 *)(actor + 48) = 0x30000;
    actor[0x55] &= 0x7e;
    *(s32 *)(actor + 52) = 0x20000;
    *(s32 *)(actor + 40) = 0x40000;

    Func_02002140(actor, 0);
    Func_020021b6(0, *(s16 *)((u8 *)target + 2),
                       *(s16 *)((u8 *)target + 10));
    Func_02002126(actor, 6);
    Func_0200215e(actor, 1);

    Func_0200219e();
    actor[0x55] = savedStatus;
}
