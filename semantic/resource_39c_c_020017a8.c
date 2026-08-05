#include "types.h"

/*
 * resource_39c owner at 0x020017a8, 336 bytes: build two eight-member
 * mirrored formations. Odd-numbered members receive independently jittered
 * coordinates and a full object setup; every member receives the common
 * placement call. Each completed formation is then registered against the
 * caller's handle before the routine closes the two-group sequence.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl/fp spill at
 * 0x020017a8 through `bx r0` at 0x020018ea. The three-word literal pool at
 * 0x020018ec-0x020018f7 belongs to this owner; the next prologue is at
 * 0x020018f8. One incoming argument (r0), used as the group handle; void.
 *
 * The previous handoff's 328-byte estimate was stale. m2c_guard independently
 * measures 324 code bytes to 0x020018ec; adding the three referenced pool
 * words gives the 336-byte whole-owner span recorded here.
 *
 * The four local callees are already bounded resource_39c owners. The ten
 * imported calls were resolved through the overlay veneers. Game-level names
 * for the presentation/setup calls are not established, so address names are
 * retained rather than guessed.
 */











static s32 FormationJitter_020017a8(u32 randomValue)
{
    u32 sample = (randomValue << 3) >> 16;
    return (s32)(sample * 0x3333) - 0xcccc;
}

extern void Func_020071ce(s32, s32, s32, s32, s32, s32);
extern void Func_020071de(s32, s32, s32, s32, s32, s32);
extern void Func_020071a6(s32, s32, s32, s32, s32, s32, u32, s32 *);
extern u32 Func_020071be(void);
extern u32 Func_02001998(void);
extern void Func_02007310(s32);
extern void Func_020053a2(s32, s32, s32);
extern void Func_020072a4(s32, s32, s32, s32, s32, s32);
extern void Func_02002e8e(s32, s32, s32);
extern void Func_02007366(s32);
extern void Func_02002ea4(s32, s32, s32);
extern void Func_0200752e(s32);
extern void Func_02007258(void *, s32);
extern void Func_0200731c(void);
void Func_020017a8(s32 handle)
{
    s32 extra[4];
    u32 group;

    Func_020071ce(78, 59, 110, 36, 1, 1);
    Func_020071de(76, 59, 109, 36, 1, 1);

    extra[1] = 7;
    extra[2] = 0x8000;
    extra[3] = 0x8000;

    for (group = 0; group <= 1; group++) {
        s32 baseX = 0x2d80000 - group * 0x100000;
        u32 member;

        for (member = 0; member <= 7; member++) {
            s32 mirrored = -member;

            if ((member & 1) != 0) {
                Func_020071a6(
                    0x2480000,
                    0,
                    baseX,
                    FormationJitter_020017a8(Func_020071be()),
                    FormationJitter_020017a8(Func_02001998()),
                    0,
                    0x90000,
                    extra);
                Func_02007310(1);
            }

            Func_020053a2(
                0x2d80000 + ((mirrored - group * 16) << 16),
                0,
                0x2480000);
            baseX -= 0x10000;
        }

        Func_020072a4(76, 59, 108 - group, 36, 2, 1);
        Func_02002e8e(handle, group, group + 1);
    }

    Func_02007366(handle);
    Func_02002ea4(0, 2, 3);
    Func_0200752e(0xd3);
    Func_02007258((void *)0x02009791, 3200);
    Func_0200731c();
}
