/* Contiguous unnamed state-owner run for resource_3b1. */

#include "types.h"

s32 Func_020064e6(u8 *o);

#define StagedActor_SetAngleByKind Func_020000b0

s32 StagedActor_SetAngleByKind(u8 *o) {
    u32 v = (u32)(Func_020064e6(o) << 6) >> 16;

    if (v == 6) {
        s32 k = 0x3000;
        *(u16 *)(o + 6) = k;
    } else if (v == 9) {
        s32 k = 0x5000;
        *(u16 *)(o + 6) = k;
    }
    return 1;
}

#include "types.h"

#define StagedActor_AdvanceCounter98 Func_020000d8

void StagedActor_AdvanceCounter98(u8 *o) {
    u8 *p = o + 98;
    s32 n = *p + 1;

    *p = n;
    if ((u8)n > 80) {
        *(u16 *)(o + 102) += 1;
    }
}

#include "types.h"

#define StagedActor_CountdownUntilPositionUnset Func_020000fc

s32 StagedActor_CountdownUntilPositionUnset(u8 *o) {
    s32 n = *(s32 *)(o + 76);

    if (n != 0) {
        *(s32 *)(o + 76) = n - 1;
    } else {
        return 1;
    }
    if (*(s32 *)(o + 56) == 0x80000000
        && *(s32 *)(o + 60) == *(s32 *)(o + 56)
        && *(s32 *)(o + 64) == *(s32 *)(o + 60)) {
        return 1;
    }
    return 0;
}
