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

u8 *Func_0808a080();
s32 Func_080770c0();
void Func_08000128();
s32 Func_080091d8();
void Func_0808a018();
void Func_0808a020();
void Func_08009080();
void Func_080000c0();
void Func_080f9010();
void Func_080091e0();
void Func_0808a0c0();

void Func_02000d0c(void)
{
    u8 *actor = Func_0808a080(0);
    u8 savedStatus = actor[0x55];
    s32 target[3];
    u16 heading;

    if (Func_080770c0(0x200) == 0) {
        return;
    }

    target[0] = (*(s32 *)(actor + 8) & (s32)0xfff00000) + 0x80000;
    target[1] = *(s32 *)(actor + 12);
    target[2] = (*(s32 *)(actor + 16) & (s32)0xfff00000) + 0x80000;

    heading = (u16)((*(u16 *)(actor + 6) + 0x2000) & 0xc000);
    Func_08000128(0x200000, heading, target);

    if (Func_080091d8(actor, target) != 0) {
        return;
    }

    Func_0808a018();
    Func_08009080(actor, 6);
    Func_080000c0(6);
    Func_080f9010(152);
    Func_08009080(actor, 7);

    *(s32 *)(actor + 48) = 0x30000;
    actor[0x55] &= 0x7e;
    *(s32 *)(actor + 52) = 0x20000;
    *(s32 *)(actor + 40) = 0x40000;

    Func_080091e0(actor, 0);
    Func_0808a0c0(0, *(s16 *)((u8 *)target + 2),
                       *(s16 *)((u8 *)target + 10));
    Func_08009080(actor, 6);
    Func_080091e0(actor, 1);

    Func_0808a020();
    actor[0x55] = savedStatus;
}
