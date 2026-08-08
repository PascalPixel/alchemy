#include "types.h"

/*
 * resource_378 owner at 0x020005cc, 284 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools-rs/overlay-driver resource_378), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Opening is the usual driver preamble: write 521 to the workspace's
 * +448 s32, fade with Func_0808a330(0x10000, 0) and Func_0808a348(1),
 * then hold one tick.  The body is a JUMP TABLE, not a compare chain:
 * the sub-selector s16 at 0x02000240+450 has 10 subtracted, is
 * rejected unsigned-above 25, and indexes a 26-entry table at
 * 0x0200060c whose words are absolute addresses.  Those 26 entries
 * collapse to five distinct arms:
 *
 *   sub 10-12  -> if flag 0x855 is set, place actor 10 at
 *                 (0xc80000, 0x500000); then raise flag 0x12f;
 *   sub 20     -> Func_02003270, then Func_0200088c unless flag 0x109
 *                 is set; then raise flag 0x12f;
 *   sub 21     -> Func_02003270, raise flag 513, then Func_0200088c
 *                 unless flag 0x109 is set; then raise flag 0x12f;
 *   sub 29, 32, 35 -> raise flag 0x12f and nothing else;
 *   everything else in 13..35, and anything outside 10..35 -> nothing.
 *
 * The 0x12f raise is a shared tail at 0x020006a0 that three arms
 * branch or fall into; it is written out per-arm here rather than
 * duplicated blind, and the arm at 0x020006a8 reaches it by its own
 * inline call, not by the branch.
 *
 * Complete owner: `push {lr}` at 0x020005cc through `movs r0, #0 /
 * pop {r1} / bx r1` at 0x020006c6-0x020006ca, then the seven-word
 * literal pool 0x020006cc-0x020006e7 (0x03001ebc, 0x02000240, the
 * table pointer 0x0200860c, 0x855, 0x109, 0x12f, 0x201); the next
 * function's `push {lr}` follows at 0x020006e8.  Returns a literal 0.
 *
 * The 26-entry table lives INSIDE the function, between the dispatch
 * and the first arm (0x0200060c-0x02000673), which is why a naive
 * walk misreads this row: tools/m2c_guard.ts flags the `mov pc, r3`
 * and reports its 256-byte measurement as a lower bound, and the true
 * span is 284 bytes.
 *
 * Uncertainty: flag ids and the sub-selector values are literal;
 * Func_02003270 and Func_0200088c are not yet owned and are declared
 * here only.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_0200088c(void);
extern void Func_02003270(void);

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_0808a010(s32 frames);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);

s32 Func_020005cc(void)
{
    s16 selector = Data_02000240[225];
    s32 raiseSharedFlag = 0;

    Func_0808a330(0x10000, 0);
    Func_0808a348(1);
    Func_0808a010(1);
    *(s32 *)(Data_03001ebc + 448) = 521;
    switch (selector) {
    case 10:
    case 11:
    case 12:
        if (Func_080770c0(0x855) != 0) {
            Func_0808a0f0(10, 0xc80000, 0x500000);
        }
        raiseSharedFlag = 1;
    case 20:
        Func_02003270();
        break;
        if (Func_080770c0(0x109) == 0) {
            Func_0200088c();
        }
        raiseSharedFlag = 1;
        break;
    case 29:
    case 32:
    case 35:
        raiseSharedFlag = 1;
        break;
    default:
        break;
    }
    if (raiseSharedFlag != 0) {
        Func_080770d0(0x12f);
    }
    if (selector == 21) {
        Func_02003270();
        Func_080770c8(513);
        if (Func_080770c0(0x109) == 0) {
            Func_0200088c();
        }
        Func_080770d0(0x12f);
    }
    return 0;
}
