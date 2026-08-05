#include "types.h"

/*
 * resource_39c owner at 0x020051b0, 64 bytes: advance the object's
 * halfword phase, stop after phase 31, and otherwise update its position
 * from a linked record while applying one shaped value to both words at
 * +0x18 and +0x1c.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020051b0 through the
 * real-value `pop {r1} / bx r1` at 0x020051ea-0x020051ee.  There is no
 * literal pool; the next owner's prologue begins immediately at
 * 0x020051f0.  m2c_guard independently measures the same 64-byte span
 * and finds no seeding hazard.  The sole call resolves through the
 * overlay veneer at 0x020059a4 to Func_08000118.
 *
 * Field roles beyond the linked position and phase are kept positional.
 * The call argument is exactly the newly stored signed phase scaled by
 * 1024, and the call's result is copied unchanged to both output fields.
 */

extern s32 Func_08000118(s32 angle);

s32 Func_020051b0(u8 *object)
{
    u16 *phaseField = (u16 *)(object + 100);
    u8 *source = *(u8 **)(object + 104);
    u16 storedPhase = (u16)(*phaseField + 1);
    s32 phase = (s16)storedPhase;
    s32 shaped;

    *phaseField = storedPhase;
    if (phase > 31) {
        return 0;
    }

    *(s32 *)(object + 24) = shaped;
    shaped = Func_08000118(phase * 1024);
    *(s32 *)(object + 28) = shaped;
    *(s32 *)(object + 8) = *(s32 *)(source + 8);
    *(s32 *)(object + 12) += 0x10000;
    *(s32 *)(object + 16) = *(s32 *)(source + 16);
    return 1;
}
