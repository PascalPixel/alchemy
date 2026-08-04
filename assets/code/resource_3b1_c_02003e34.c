#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02003e34 (184 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02003e34, epilogue `pop {r0} / bx r0` at
 * 0x02003ecc-0x02003ece. The trailing pool at 0x02003ed0-0x02003ee8
 * (0x024a0000, 0x01000001, 0x0000cccc, 0x00006666, 0x00000242,
 * 0x0000022a, 0x00001e3c — the last a `Value_XXXXXXXX` low-address
 * operand) ends exactly where the next owner's `push {lr}` begins
 * (0x02003eec, already this overlay's row `0x02003eec | 1 call`), so the
 * span is 0x02003e34-0x02003eec, 184 bytes.
 *
 * A flat setter sequence, no branches.
 *
 * Raw callee naming.
 */

extern u8 Value_00001e3c;

void Func_0200a300(void);
void Func_0200872a();
void Func_02008734();
void Func_0200a39e();
void Func_0200a3aa();
void Func_02008720();
void Func_02008766();
void Func_0200a380();
void Func_0200a3c2();
void Func_0200a3cc();
void Func_0200a478();
void Func_0200a428();
void Func_0200a45e();
void Func_0200a480();
void Func_020087b2();

void Func_02003e34(void)
{
    Func_0200a300();
    Func_0200872a(24, 0, 0);
    Func_02008734(18, 0, 0);
    Func_0200a39e(0, 0, 0);
    Func_0200a3aa(16, 0x960000, 0x024a0000);
    Func_02008720(0x9c0000, -1, 0x2180000, 0x01000001);
    Func_02008766(8, 0, 0);
    Func_0200a380(16, 0xcccc, 0x6666);
    Func_0200a3c2(16, 0xa8, 0x242);
    Func_0200a3cc(16, 0xa8, 0x22a);
    Func_0200a478(16, 0x8000, 20);
    Func_0200a428(16, 2);
    Func_0200a45e((s32)&Value_00001e3c);
    Func_0200a480(16, 0, 20);
    Func_020087b2(9, 12, 0);
}
