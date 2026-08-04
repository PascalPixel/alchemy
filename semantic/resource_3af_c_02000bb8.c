#include "types.h"

/*
 * resource_3af owner at 0x02000bb8, 56 bytes: a once-only reward grant
 * guarded by a story flag. Tests flag 0x271 (Func_080770c0, the
 * test/set pair idiom); if unset, plays sound 158 (Func_080f9010),
 * calls the six-argument Func_08009180(30, 94, 13, 94, 1, 3) -- the
 * last two arguments passed on the stack -- then sets flag 0x271
 * (Func_080770c8). Called 9x within this overlay, the highest call
 * count in its population.
 *
 * Byte-identical twin of this overlay's own 0x02000bf0 apart from the
 * flag word (0x271 vs 0x272) and two of the six arguments (94/3 vs
 * 108/2); see resource_3af_c_02000bf0.c.
 *
 * Complete owner: `push {lr} / sub sp, #8` at 0x02000bb8-0x02000bbc
 * through `add sp, #8 / pop {r0} / bx r0` at 0x02000be6-0x02000bea,
 * one pool word (0x00000271) plus an alignment halfword ending
 * 0x02000bef; the next owner prologue (the twin) is immediately at
 * 0x02000bf0.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_3af bb8 c28's
 * +2 rule).
 *
 * Uncertainty: callees identified by shape; the six argument roles of
 * Func_08009180 are unknown, transcribed in register/stack order per
 * the existing precedent signature.
 */

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 sound_id);
extern void Func_08009180(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02000bb8(void)
{
    if (Func_080770c0(0x271) == 0) {
        Func_080f9010(158);
        Func_08009180(30, 94, 13, 94, 1, 3);
        Func_080770c8(0x271);
    }
}
