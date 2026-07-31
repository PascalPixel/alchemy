typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02004198 (128 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x02004198, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x0200420a-0x0200420e. The two-word pool at 0x02004210-0x02004214
 * (0x0200e840, 0x0200e8e4, both handle/pointer values) is included per
 * the usual pool rule, immediately followed by the next owner's push
 * {lr} at 0x02004218, already this overlay's row `0x02004218 | 1 call`,
 * so the span is 0x02004198-0x02004218, 128 bytes.
 *
 * A flat setter sequence, no branches. The final call
 * (`Func_0200a6d8`, same target as an earlier call in this function)
 * carries no explicit argument setup in the reference, so it is spelled
 * with no arguments here too.
 *
 * Raw callee naming.
 */

void Func_0200a664(void);
void Func_02008a8e();
void Func_02008a98();
void Func_02007968(void);
void Func_02008aa6();
void Func_0200a716();
void Func_0200a6ce();
void Func_0200a6d8();
void Func_0200a6e0();
void Func_0200a6e8();
void Func_0200a788();
void Func_0200a790();
void Func_0200a798();
void Func_020092e8(void);

void Func_02004198(void)
{
    Func_0200a664();
    Func_02008a8e(24, 1, 0);
    Func_02008a98(25, 1, 0);
    Func_02007968();
    Func_02008aa6(19, 11, 12);
    Func_0200a716(10, 6);
    Func_0200a6ce(12, 0x0200e840);
    Func_0200a6d8(0x24, 0x0200e8e4);
    Func_0200a6e0(0x25, 0x0200e8e4);
    Func_0200a6e8(0x26, 0x0200e8e4);
    Func_0200a788(0x24, 3);
    Func_0200a790(0x25, 3);
    Func_0200a798(0x26, 3);
    Func_020092e8();
    Func_0200a6d8();
}
