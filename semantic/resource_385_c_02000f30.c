#include "types.h"

/*
 * resource_385 owner at 0x02000f30, 296 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_385), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Shape, all of it flag- and selector-gated:
 * - flag 512 set -> pop panel (55, 26, 4, 2, 23, 26);
 * - always: Func_020000a0(0x800000, 0, 0x1a40000, 223) -- the exact-C
 *   owner in this overlay -- then strip (45, 41, 8, 45, 3, 3) and a
 *   Func_080000c0(1) refresh;
 * - hang callback 0x02008401 on records 14 and 15 (+108) and set
 *   their +100 u16 to 1 and 0 respectively;
 * - flag 0x858 set -> place record 18 at (0xd80000, 0x1880000);
 * - sub-selector <= 2, with flags 52 and 0x109 both clear -> raise
 *   flag 0x867;
 * - flag 0x867 set and flag 52 clear -> place record 21 at
 *   (0x1980000, 0x780000);
 * - sub-selector 11 -> raise flag 0x12f;
 * - sub-selector 13 -> raise flag 288.
 * Returns 0.
 *
 * The last two tests read the SAME halfword at Data_02000240[225],
 * and the reference re-reads it after the 0x12f raise rather than
 * reusing the register -- so the second test sees any change that
 * raise caused.  Reading the cell at each use keeps that behaviour;
 * it is not a redundant load folded away.  The final test is an
 * unsigned `ldrh` in the reference, so it is spelled through a u16
 * pointer rather than as the array's signed element.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000f30 with `sub sp, #8`
 * through `movs r0, #0 / add sp, #8 / pop {r5, r6} / pop {r1} /
 * bx r1` at 0x02001036-0x0200103e, then the six-word literal pool
 * 0x02001040-0x02001057 (0x02008401, 0x858, 0x02000240, 0x109,
 * 0x867, 0x12f); the overlay's import-veneer table begins at
 * 0x02001058.  Returns a literal 0.
 *
 * Cross-checked per the 2026-08-01 ruling: tools/m2c_guard.ts clears
 * this row of the call-through-register hazard and reports full call
 * coverage against this draft.
 *
 * Uncertainty: flag ids and selector values are literal; record
 * fields +100 and +108 are recorded by offset, and 0x02008401 is an
 * in-image code pointer (image offset 0x400 | 1) installed as a
 * callback, not called here.
 */

extern s16 Data_02000240[];
extern void Func_020000a0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern void Func_080000c0(s32 arg0);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);

s32 Func_02000f30(void)
{
    if (Func_080770c0(512) != 0) {
        Func_080091c0(55, 26, 4, 2, 23, 26);
    }
    Func_020000a0(0x800000, 0, 0x1a40000, 223);
    Func_08009180(45, 41, 8, 45, 3, 3);
    Func_080000c0(1);
    *(s32 *)(Func_0808a080(14) + 108) = 0x02008401;
    *(u16 *)(Func_0808a080(14) + 100) = 1;
    *(s32 *)(Func_0808a080(15) + 108) = 0x02008401;
    *(u16 *)(Func_0808a080(15) + 100) = 0;
    if (Func_080770c0(0x858) != 0) {
        Func_0808a0f0(18, 0xd80000, 0x1880000);
    }
    if (Data_02000240[225] <= 2
        && Func_080770c0(52) == 0
        && Func_080770c0(0x109) == 0) {
        Func_080770d0(0x867);
    }
    if (Func_080770c0(0x867) != 0 && Func_080770c0(52) == 0) {
        Func_0808a0f0(21, 0x1980000, 0x780000);
    }
    if (Data_02000240[225] == 11) {
        Func_080770d0(0x12f);
    }
    if (*(u16 *)&Data_02000240[225] == 13) {
        Func_080770d0(288);
    }
    return 0;
}
