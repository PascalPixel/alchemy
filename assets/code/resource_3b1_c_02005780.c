#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02005780 (108 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` at 0x02005780, epilogue `pop {r5,r6} / pop {r0} / bx r0`
 * at 0x020057e0-0x020057e4. The one-word literal pool at 0x020057e8
 * (0x0000092a, the `Value_XXXXXXXX` family used throughout this
 * overlay) is included per the usual pool rule, immediately followed by
 * the next owner's push {r5,r6,lr} at 0x020057ec, already this
 * overlay's row `0x020057ec | 1 call`, so the span is
 * 0x02005780-0x020057ec, 108 bytes.
 *
 * A flat setter sequence, no branches. A close sibling of 0x020056dc
 * (same overall shape, one fewer field-setup pair).
 *
 * Per-site call veneers (raw asm confirms each callee slot uses its own
 * local stub, distinct from the generic main-image symbol name).
 */

extern u8 Value_0000092a;

s32 Func_0200a730();
s32 Func_0200a73a();
void Func_0200bc60(void);
void Func_0200a08a();
void Func_0200a094();
void Func_02008f8a();
void Func_0200a0a4();
void Func_0200bd0e();
void Func_0200bd18();
void Func_0200aac8(void);
void Func_0200bc82();
void Func_0200bcae(void);

void Func_02005780(void)
{
    s32 handle = Func_0200a730(0, 0);
    s32 other = Func_0200a73a(1, 0);

    Func_0200bc60();
    Func_0200a08a(24, 1, 0);
    Func_0200a094(25, 0, 0);
    Func_02008f8a(0);
    Func_0200a0a4(19, handle, other);
    Func_0200bd0e(11, 0, 0);
    Func_0200bd18(12, 0, 0);
    Func_0200aac8();
    Func_0200bc82((s32)&Value_0000092a);
    Func_0200bcae();
}
