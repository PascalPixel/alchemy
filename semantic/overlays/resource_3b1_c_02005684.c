typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02005684 (88 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x02005684, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x020056d0-0x020056d4. The one-word literal pool at 0x020056d8
 * (0x00000929, the `Value_XXXXXXXX` family used throughout this
 * overlay) is included per the usual pool rule, immediately followed by
 * the next owner's push {r5,r6,lr} at 0x020056dc, already this
 * overlay's row `0x020056dc | 1 call`, so the span is
 * 0x02005684-0x020056dc, 88 bytes.
 *
 * A flat setter sequence, no branches.
 *
 * Raw callee naming.
 */

extern u8 Value_00000929;

s32 Func_0200a634();
void Func_0200bb5a(void);
void Func_02009f84();
void Func_02009f8e();
void Func_02008e84();
void Func_02009f9e();
void Func_0200bc08();
void Func_0200a9b8(void);
void Func_0200bb72();
void Func_0200bb9e(void);

void Func_02005684(void)
{
    s32 r5 = Func_0200a634(0, 0);

    Func_0200bb5a();
    Func_02009f84(24, 1, 0);
    Func_02009f8e(25, 0, 0);
    Func_02008e84(0);
    Func_02009f9e(19, r5, 12);
    Func_0200bc08(11, 0, 0);
    Func_0200a9b8();
    Func_0200bb72((s32)&Value_00000929);
    Func_0200bb9e();
}
