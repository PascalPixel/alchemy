typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02004218 (60 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02004218, epilogue `pop {r0} / bx r0` at
 * 0x0200424a-0x0200424c. The one-word literal pool at 0x02004250
 * (0x00000928, same `Value_XXXXXXXX` family as elsewhere in this
 * overlay) is included per the usual pool rule, immediately followed by
 * the next owner's push at 0x02004254 (the still-parked
 * `0x02004254 | 4 calls` row — this confirms that row's start boundary
 * from the other side), so the span is 0x02004218-0x02004254, 60 bytes.
 *
 * A flat setter sequence, no branches. The final call
 * (`Func_0200a718`) carries no explicit argument setup in the reference.
 *
 * Raw callee naming.
 */

extern u8 Value_00000928;

void Func_0808a018(void);
void Func_020048e8();
void Func_020052f4(void);
void Func_080770c8();
void Func_0808a020(void);

void Func_02004218(void)
{
    Func_0808a018();
    Func_020048e8(24, 1, 0);
    Func_020048e8(25, 0, 0);
    Func_020048e8(19, 11, 12);
    Func_020052f4();
    Func_080770c8((s32)&Value_00000928);
    Func_0808a020();
}
