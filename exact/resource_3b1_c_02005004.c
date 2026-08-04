/*
 * Resource 3b1 unindexed helper at 0x02005004 (52 bytes incl. pool,
 * 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02005004, epilogue `pop {r0} / bx r0` at
 * 0x02005028-0x0200502a. The trailing pool at 0x0200502c-0x02005034
 * (0x0000092c, 0x00000935, 0x00000917 -- plain numeric arguments, the
 * same constants seen as case values at 0x02004fa8 elsewhere in this
 * overlay, not the address-of `Value_XXXXXXXX` idiom) is included per
 * the usual pool rule, immediately followed by the next owner's push
 * {r5,r6,r7,lr} at 0x02005038, already this overlay's row
 * `0x02005038 | 4 calls`, so the span is 0x02005004-0x02005038,
 * 52 bytes.
 *
 * A flat setter sequence, no branches.
 *
 * Raw callee naming.
 */

void Func_0200a044();
void Func_0200a04c();
void Func_0200a054();
void Func_0200a05e();

void Func_02005004(void)
{
    Func_0200a044(8, 0x92c);
    Func_0200a04c(8, 0x935);
    Func_0200a054(8, 0x917);
    Func_0200a05e(8, 0x990);
}
