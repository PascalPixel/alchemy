#include "types.h"

/*
 * resource_3bb owner at 0x020029d0, 254 bytes: a two-branch setup
 * sequence keyed on `arg0 == 0` -- each branch issues its own long,
 * mostly linear run of fixed-id setup calls; the non-zero branch also
 * writes a per-`arg0` table entry and spin-waits on a poll callee before
 * a further run of id/index-pair calls.
 *
 * Complete owner: `push {r5, lr}` at 0x020029d0 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02002ac8-0x02002acc, followed by the three-word
 * literal pool 0x02002ad0-0x02002adb; the next owner's prologue is at
 * 0x02002adc (this overlay's own resource_3bb_c_02002adc.c candidate,
 * not yet drafted). One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * The `while (Func_080000c0() != 0) Func_080f9048(1);` loop is the
 * compiled "jump to test first" shape (`b.n` straight to the test before
 * ever running the body), not a `do`.
 *
 * Uncertainty: none of the thirty-odd callees are identified beyond call
 * shape; several calls in both branches reuse the previous call's
 * literal argument in `r0` without reloading it, and are declared
 * old-style (no visible argument) rather than credited with a stale
 * value, per this project's "arities vary per site" convention
 * (resource_3cb_c_02000580.c) -- distinct from a genuine chained return
 * value, which this owner does not exhibit.
 */














extern void Func_02006912(void);
extern void Func_02006a3e(void);
extern void Func_02006a52(void);
extern void Func_02006918(s32 arg0);
extern void Func_02006a9e(s32 arg0);
extern void Func_02004db0(s32 arg0);
extern void Func_0200533c(s32 arg0, s32 arg1);
extern void Func_02006932(s32 arg0);
extern void Func_02006946(void);
extern void Func_02006abe(s32 arg0);
extern void Func_0200694a(void);
extern void Func_02006a76(void);
extern void Func_02006a8a(void);
extern void Func_0200695a(s32 arg0);
extern void Func_02006ae2(s32 arg0);
extern void Func_02004df4(s32 arg0);
extern void Func_02005380(s32 arg0, s32 arg1);
extern void Func_02006976(s32 arg0);
extern s32 Func_020067ee(void);
extern void Func_02006b0a(s32 arg0);
extern void Func_02006b0c(s32 arg0);
extern void Func_02004e1e(s32 arg0);
extern void Func_020053aa(s32 arg0, s32 arg1);
extern void Func_02006b20(s32 arg0);
extern void Func_020069a6(s32 arg0);
extern void Func_020053be(s32 arg0, s32 arg1);
extern void Func_02006b34(s32 arg0);
extern void Func_020069ba(s32 arg0);
extern void Func_02004e4c(s32 arg0);
extern void Func_020053d8(s32 arg0, s32 arg1);
extern void Func_02006b4e(s32 arg0);
extern void Func_020069d4(s32 arg0);
extern void Func_02004e66(s32 arg0);
extern void Func_020053f2(s32 arg0, s32 arg1);
extern void Func_02006b68(s32 arg0);
extern void Func_02006b44(void);
extern void Func_02006a00(void);
extern void Func_020069b6(s32 arg0);
void Func_020029d0(s32 arg0)
{
    if (arg0 == 0) {
        Func_02006912();
        Func_02006a3e();
        Func_02006a52();
        Func_02006918(30);
        Func_02006a9e(89);
        Func_02004db0(0);
        Func_0200533c(1, 0);
        Func_02006932(120);
        Func_02006946();
        return;
    }

    Func_02006abe(247);
    Func_0200694a();
    Func_02006a76();
    Func_02006a8a();

    *(u16 *)0x0200ca3c = (u16)(arg0 * 60);

    Func_0200695a(30);
    Func_02006ae2(arg0 + 0x5a);
    Func_02004df4(arg0);
    Func_02005380(1, 0);
    Func_02006976(120);

    while (Func_020067ee() != 0) {
        Func_02006b0a(1);
    }

    Func_02006b0c(0x121);
    Func_02004e1e(5);
    Func_020053aa(2, 0);
    Func_02006b20(236);
    Func_020069a6(60);
    Func_020053be(2, 1);
    Func_02006b34(236);
    Func_020069ba(60);
    Func_02004e4c(6);
    Func_020053d8(2, 0);
    Func_02006b4e(236);
    Func_020069d4(60);
    Func_02004e66(7);
    Func_020053f2(4, 0);
    Func_02006b68(237);
    Func_02006b44();
    Func_02006a00();
    Func_020069b6(0x123);
}
