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

extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern u32 Func_080000f8(void);
extern void Func_0200013c(s32, s32, s32, s32, s32, s32, u32, s32 *);
extern void Func_0808a010(s32);
extern void Func_02003b24(s32, s32, s32);
extern void Func_020015e0(s32, s32, s32);
extern void Func_080f9010(s32);
extern void Func_080000d0(void *, s32);
extern void Func_080091f8(void);

static s32 FormationJitter_020017a8(u32 randomValue)
{
    u32 sample = (randomValue << 3) >> 16;
    return (s32)(sample * 0x3333) - 0xcccc;
}

void Func_020017a8(s32 handle)
{
    s32 extra[4];
    u32 group;

    Func_08009180(78, 59, 110, 36, 1, 1);
    Func_08009180(76, 59, 109, 36, 1, 1);

    extra[1] = 7;
    extra[2] = 0x8000;
    extra[3] = 0x8000;

    for (group = 0; group <= 1; group++) {
        s32 baseX = 0x2d80000 - group * 0x100000;
        u32 member;

        for (member = 0; member <= 7; member++) {
            s32 mirrored = -member;

            if ((member & 1) != 0) {
                Func_0200013c(
                    0x2480000,
                    0,
                    baseX,
                    FormationJitter_020017a8(Func_080000f8()),
                    FormationJitter_020017a8(Func_080000f8()),
                    0,
                    0x90000,
                    extra);
                Func_0808a010(1);
            }

            Func_02003b24(
                0x2d80000 + ((mirrored - group * 16) << 16),
                0,
                0x2480000);
            baseX -= 0x10000;
        }

        Func_08009180(76, 59, 108 - group, 36, 2, 1);
        Func_020015e0(handle, group, group + 1);
    }

    Func_0808a010(handle);
    Func_020015e0(0, 2, 3);
    Func_080f9010(0xd3);
    Func_080000d0((void *)0x02009791, 3200);
    Func_080091f8();
}
