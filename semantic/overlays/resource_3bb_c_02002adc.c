typedef unsigned short u16;
typedef int s32;

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
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * The `while (Func_02006c28() != 0) Func_0200690c(1);` loop is the
 * compiled "jump to test first" shape, like
 * resource_3bb_c_020029d0.c's poll loop.
 *
 * Uncertainty: none of the seventeen callees are identified beyond call
 * shape; the two `0x0200cXXX` cells are recorded as opaque addresses.
 */

extern void Func_02006b94(s32 arg0);
extern void Func_02006b48();
extern void Func_02006b5c();
extern void Func_02006a42(s32 arg0);
extern void Func_02006bc8(s32 arg0);
extern void Func_02004eda(s32 arg0);
extern void Func_02005466(s32 arg0, s32 arg1);
extern void Func_02006a62(s32 arg0);
extern void Func_02006a6c(s32 arg0);
extern void Func_02006bf4(s32 arg0);
extern void Func_02004f06(s32 arg0);
extern void Func_02005492(s32 arg0, s32 arg1);
extern void Func_02006a8a(s32 arg0);
extern void Func_02006b64(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200690c(s32 arg0);
extern s32 Func_02006c28(void);
extern void Func_02006c2a(s32 arg0);
extern void Func_02006ab0(s32 arg0);
extern void Func_02006c36(s32 arg0);
extern void Func_02006bf2();
extern void Func_02006bfe();

void Func_02002adc(s32 arg0)
{
    s32 absScaled = (arg0 < 0 ? -arg0 : arg0) * 60;
    s32 trailingArg;

    Func_02006b94(247);
    Func_02006b48();
    Func_02006b5c();

    *(u16 *)(0x0200cac8 + 26) = (u16)(arg0 * 60);
    *(u16 *)(0x0200c0f6 + 26) = (u16)absScaled;

    if (arg0 < 0) {
        Func_02006a42(30);
        Func_02006bc8(86);
        Func_02004eda(8);
        Func_02005466(3, 1);
        Func_02006a62(-60 * arg0 + 60);
        trailingArg = 0;
    } else {
        Func_02006a6c(30);
        Func_02006bf4(arg0 + 0x5a);
        Func_02004f06(4);
        Func_02005492(3, 0);
        Func_02006a8a(absScaled + 60);
        trailingArg = 8;
    }

    Func_02006b64(trailingArg, 0x105, 0);

    while (Func_02006c28() != 0) {
        Func_0200690c(1);
    }

    Func_02006c2a(19);
    Func_02006ab0(30);
    Func_02006c36(0x121);
    Func_02006bf2();
    Func_02006bfe();
}
