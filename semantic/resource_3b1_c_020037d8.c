#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x020037d8 (212 bytes incl. pool,
 * 4 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x020037d8, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x0200389a-0x0200389e. The trailing pool at 0x020038a0-0x020038a8
 * (0x00000929, 0x0000092a, 0x0000092b) is included per the usual pool
 * rule, immediately followed by the next owner's push
 * {r5,r6,r7,lr} at 0x020038ac, already this overlay's row
 * `0x020038ac | 1 call`, so the span is 0x020037d8-0x020038ac,
 * 212 bytes.
 *
 * Four sequential "guard, then setter pair" stages: the first three
 * guards early-return only when `obj != 0` (a `NULL` object skips the
 * guard and always proceeds); the fourth guard is unconditional on
 * `obj != 0` alone (no `Value_XXXXXXXX` check), so the final setter pair
 * only ever runs when `obj == 0`.
 *
 * Raw callee naming.
 */

extern u8 Value_00000929;
extern u8 Value_0000092a;
extern u8 Value_0000092b;






extern s32 Func_02009c84();
extern s32 Func_02008798();
extern void Func_02008092();
extern void Func_02009d54();
extern s32 Func_02009cb6();
extern s32 Func_020087ca();
extern void Func_020080c4();
extern void Func_02009d86();
extern s32 Func_02009ce8();
extern s32 Func_020087fc();
extern void Func_020080f6();
extern void Func_02009db8();
extern s32 Func_02008824();
extern void Func_0200811e();
extern void Func_02009de0();
void Func_020037d8(u8 *obj)
{
    if (obj != 0 && Func_02009c84((s32)&Value_00000929) == 0) {
        return;
    }

    if (Func_02008798(0, 0) != 0) {
        Func_02008092(0x19a, 0xac, 0xd000);
        Func_02009d54(10, 0, 0);
    }

    if (obj != 0 && Func_02009cb6((s32)&Value_0000092a) == 0) {
        return;
    }

    if (Func_020087ca(1, 0) != 0) {
        Func_020080c4(0x1d6, 0xac, 0xb000);
        Func_02009d86(11, 0, 0);
    }

    if (obj != 0 && Func_02009ce8((s32)&Value_0000092b) == 0) {
        return;
    }

    if (Func_020087fc(2, 0) != 0) {
        Func_020080f6(0x19a, 0xcc, 0xd000);
        Func_02009db8(12, 0, 0);
    }

    if (obj != 0) {
        return;
    }

    if (Func_02008824(3, 0) != 0) {
        Func_0200811e(0x1d6, 0xcc, 0xb000);
        Func_02009de0(13, 0, 0);
    }
}
