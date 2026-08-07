#include "types.h"

/*
 * resource_3af owner at 0x02000bf0, 56 bytes: byte-identical twin of
 * this overlay's 0x02000bb8 (see resource_3af_c_02000bb8.c) apart from
 * the flag word and two arguments. Tests story flag 0x272
 * (Func_080770c0); if unset, plays sound 158, calls
 * Func_08009180(30, 108, 13, 108, 1, 2), then sets flag 0x272.
 * Called 3x within this overlay.
 *
 * Complete owner: `push {lr} / sub sp, #8` at 0x02000bf0-0x02000bf4
 * through `add sp, #8 / pop {r0} / bx r0` at 0x02000c1e-0x02000c22,
 * one pool word (0x00000272) ending 0x02000c27; the next owner
 * prologue (`push {r5, r6, lr}`) begins at 0x02000c28.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/lib/overlay_call_targets.ts resource_3af bb8 c28's
 * +2 rule).
 *
 * Uncertainty: as for the twin -- callees by shape, Func_08009180's
 * six argument roles unknown, transcribed in register/stack order.
 */

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 sound_id);
extern void Func_08009180(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02000bf0(void)
{
    if (Func_080770c0(0x272) == 0) {
        Func_080f9010(158);
        Func_08009180(30, 108, 13, 108, 1, 2);
        Func_080770c8(0x272);
    }
}
