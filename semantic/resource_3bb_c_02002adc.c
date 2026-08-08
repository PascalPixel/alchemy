#include "types.h"

/*
 * resource_3bb owner at 0x02002adc, 186 bytes: seed two fixed-cell
 * scale values (one signed by `arg0`'s sign, one by its absolute value),
 * then run one of two direction-specific setup sequences before a
 * shared closing sequence and a poll spin-loop.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002adc through `pop
 * {r5, r6} / pop {r0} / bx r0` at 0x02002b90-0x02002b94, followed by the
 * four-word literal pool 0x02002b98-0x02002ba7; the next owner's
 * prologue is at 0x02002ba8 (this overlay's 7-call candidate
 * 0x02002ba8, a separate item-28 row, not yet drafted). One argument
 * (r0), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * The `while (Func_080000c0() != 0) Func_080f9048(1);` loop is the
 * compiled "jump to test first" shape, like
 * resource_3bb_c_020029d0.c's poll loop.
 *
 * Uncertainty: none of the seventeen callees are identified beyond call
 * shape; the two `0x0200cXXX` cells are recorded as opaque addresses.
 */

extern void Func_080f9010(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 arg0);
extern void Func_020023bc(s32 arg0);
extern void Func_02002940(s32 arg0, s32 arg1);
extern void Func_0808a1e8(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_080000c0(void);
extern void Func_080f9048(s32 arg0);
extern void Func_0808a368(void);

void Func_02002adc(s32 arg0)
{
    s32 absScaled = (arg0 < 0 ? -arg0 : arg0) * 60;
    s32 trailingArg;

    Func_080f9010(247);
    Func_0808a360();
    Func_0808a370();

    *(u16 *)(0x0200c0f6 + 26) = (u16)absScaled;
    *(u16 *)(0x0200cac8 + 26) = (u16)(arg0 * 60);

    if (arg0 < 0) {
        Func_0808a010(30);
        Func_080f9010(86);
        Func_020023bc(8);
        Func_02002940(3, 1);
        Func_0808a010(-60 * arg0 + 60);
    } else {
        trailingArg = 0;
        Func_0808a010(30);
        Func_080f9010(arg0 + 0x5a);
        Func_020023bc(4);
        Func_02002940(3, 0);
        Func_0808a010(absScaled + 60);
        trailingArg = 8;
    }

    Func_0808a1e8(trailingArg, 0x105, 0);

    while (Func_080000c0() != 0) {
        Func_080f9048(1);
    }

    Func_080f9010(19);
    Func_0808a010(30);
    Func_080f9010(0x121);
    Func_0808a368();
    Func_0808a370();
}
