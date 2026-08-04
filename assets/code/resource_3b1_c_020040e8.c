#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x020040e8 (84 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x020040e8, epilogue `pop {r0} / bx r0` at
 * 0x02004134-0x02004136. The one-word literal pool at 0x02004138
 * (0x00001e40, same `Value_XXXXXXXX` low-address family as elsewhere in
 * this overlay) is included per the usual pool rule, immediately
 * followed by the next owner's push {lr} at 0x0200413c, already this
 * overlay's row `0x0200413c | 1 call`, so the span is
 * 0x020040e8-0x0200413c, 84 bytes.
 *
 * A flat setter sequence, no branches.
 *
 * Raw callee naming.
 */

extern u8 Value_00001e40;

void Func_0200a5b4(void);
void Func_020089de();
void Func_0200a67e();
void Func_0200a5c4();
void Func_0200a6e0();
void Func_0200a690();
void Func_0200a6c6();
void Func_0200a6e8();
void Func_02008a1a();

void Func_020040e8(void)
{
    Func_0200a5b4();
    Func_020089de(15, 1, 1);
    Func_0200a67e(8, 1);
    Func_0200a5c4(10);
    Func_0200a6e0(8, 0x3000, 20);
    Func_0200a690(8, 2);
    Func_0200a6c6((s32)&Value_00001e40);
    Func_0200a6e8(8, 0, 20);
    Func_02008a1a(9, 14, 0);
}
