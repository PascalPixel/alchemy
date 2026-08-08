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
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
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

extern void Func_0808a018(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 arg0);
extern void Func_080f9010(s32 arg0);
extern void Func_020023bc(s32 arg0);
extern void Func_02002940(s32 arg0, s32 arg1);
extern void Func_0808a020(void);
extern s32 Func_080000c0(void);
extern void Func_080f9048(s32 arg0);
extern void Func_0808a4f0(void);
extern void Func_080770c8(s32 arg0);

void Func_020029d0(s32 arg0)
{
    if (arg0 == 0) {
        Func_0808a018();
        Func_0808a360();
        Func_0808a370();
        Func_0808a010(30);
        Func_080f9010(89);
        Func_020023bc(0);
        Func_02002940(1, 0);
        Func_0808a010(120);
        Func_0808a020();
        return;
    }

    Func_080f9010(247);
    Func_0808a018();
    Func_0808a360();
    Func_0808a370();

    *(u16 *)0x0200ca3c = (u16)(arg0 * 60);

    Func_0808a010(30);
    Func_080f9010(arg0 + 0x5a);
    Func_020023bc(arg0);
    Func_02002940(1, 0);
    Func_0808a010(120);

    while (Func_080000c0() != 0) {
        Func_080f9048(1);
    }

    Func_080f9010(0x121);
    Func_020023bc(5);
    Func_02002940(2, 0);
    Func_080f9010(236);
    Func_0808a010(60);
    Func_02002940(2, 1);
    Func_080f9010(236);
    Func_0808a010(60);
    Func_020023bc(6);
    Func_02002940(2, 0);
    Func_080f9010(236);
    Func_0808a010(60);
    Func_020023bc(7);
    Func_02002940(4, 0);
    Func_080f9010(237);
    Func_0808a4f0();
    Func_0808a020();
    Func_080770c8(0x123);
}
