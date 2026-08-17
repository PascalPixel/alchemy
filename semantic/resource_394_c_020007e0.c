#include "types.h"

/*
 * Resource 394 cutscene at 0x020007e0.
 *
 * Complete owner: `push {r5, lr}` / `sub sp,#8` at 0x020007e0 and the matching
 * `add sp,#8 / pop {r5} / pop {r0} / bx r0` at 0x02000898.  208-byte row: 192
 * bytes of code plus the four pool words 0x00000001, 0x00003333, 0x00004ccc
 * and 0x020092c4 filling 0x020008a0-0x020008af.  The body is straight line, so
 * the control-flow walk reaches nothing at or past 0x020008a0, and the next
 * owner's prologue is at 0x020008b0.  The return address is popped into r0, so
 * the owner is `void`.
 *
 * Role.  The twelve-byte interaction records name this address through the
 * pool word 0x020087e1 (= 0x0200_07e0 plus the Thumb bit under the proven
 * 0x02008000 link base), keyed 0x00008602/0xffff000d.
 *
 * NEAR-TWIN of tracked byte-exact assets/code/resource_394_c_020008b0.c, and
 * that is this file's correctness proof.  The two owners run the same
 * twenty-one step sequence in the same order with the same imports; they
 * differ in four immediates (0x48 against 0x68 for the second placement
 * argument, -8 against 8 for the nudge, and the fifth argument of the two
 * fixture placements) and in the value finally written through
 * Data_020092c4 — 1 here, 0 there.  Resolving that tracked file's own printed
 * `bl` labels through the `+2` rule therefore names every import used here,
 * backwards: Func_02001952 is Func_0808a018, Func_02001a08 is Func_0808a020,
 * Func_020019de/02001a42/02001a48 are all Func_080f9010, and so on.  It also
 * shows two of its printed names taking different argument counts at different
 * sites, which is why the declarations below are old-style.
 *
 * The two Func_0808a090 calls carry 16.16 scale pairs — (0x8000, 0x3333) and
 * (0x4ccc, 0x3333) against 0x10000 as one, i.e. 0.5/0.2 and 0.3/0.2 — the same
 * two pairs the tracked sibling passes.
 *
 * Twenty call sites, matching the row's advertised count.
 */

extern u16 *Data_020092c4;

/* Resolved whole-owner call targets. */











extern void Func_02001882();
extern void Func_020018ca();
extern void Func_02001888();
extern void Func_0200190e();
extern void Func_020018ba();
extern void Func_020018ea();
extern void Func_020018d4();
extern void Func_020018b2();
extern void Func_02001902();
extern void Func_020018e4();
extern void Func_02001900();
extern void Func_020018d6();
extern void Func_02001926();
extern void Func_0200191c();
extern void Func_02001934();
extern void Func_02001974();
extern void Func_0200197a();
extern void Func_020018ee();
extern void Func_020018fe();
extern void Func_0200193a();
void Func_020007e0(void)
{
    Func_02001882();
    Func_020018ca(0, 8);
    Func_02001888(6);
    Func_0200190e(0xef);
    Func_020018ba(8, 0x8000, 0x3333);
    Func_020018ea(8, 2);
    Func_020018d4(8, 0x48, 0xb0);
    Func_020018b2(6);
    Func_02001902(0, 2);
    Func_020018e4(0, 0x4ccc, 0x3333);
    Func_02001900(0, -8, 0);
    Func_020018d6(24);
    Func_02001926(0, 1);
    Func_0200191c(8);
    Func_02001934(8, 1);
    Func_02001974(288);
    Func_0200197a(213);
    Func_020018ee(5, 9, 1, 4, 6, 9);
    {
        s32 four = 4;
        Func_020018fe(0, 0, 1, four, four, 9);
    }
    *Data_020092c4 = 1;
    Func_0200193a();
}
