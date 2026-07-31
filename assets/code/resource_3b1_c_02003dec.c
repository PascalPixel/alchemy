typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02003dec (72 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02003dec, epilogue `pop {r0} / bx r0` at
 * 0x02003e2a-0x02003e2c. The one-word literal pool at 0x02003e30
 * (0x00001e3d, same `Value_XXXXXXXX` low-address family as
 * assets/code/resource_3b1_c_02000b84.c's 0x1e81/0x1ea1) is included per
 * the usual pool rule (the halfword at 0x2003e2e is the alignment pad),
 * immediately followed by the next owner's push {lr} at 0x02003e34,
 * already this overlay's row `0x02003e34 | 1 call`, so the span is
 * 0x02003dec-0x02003e34, 72 bytes.
 *
 * A flat setter sequence, no branches.
 *
 * Raw callee naming.
 */

extern u8 Value_00001e3d;

void Func_0200a2b8(void);
void Func_020086e2();
void Func_0200a3d6();
void Func_0200a386();
void Func_0200a3bc();
void Func_0200a3de();
void Func_02008710();

void Func_02003dec(void)
{
    Func_0200a2b8();
    Func_020086e2(15, 1, 1);
    Func_0200a3d6(8, 0x5000, 0x28);
    Func_0200a386(8, 2);
    Func_0200a3bc((s32)&Value_00001e3d);
    Func_0200a3de(8, 0, 20);
    Func_02008710(9, 11, 0);
}
