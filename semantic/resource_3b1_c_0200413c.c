typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x0200413c (92 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x0200413c, epilogue `pop {r0} / bx r0` at
 * 0x0200418e-0x02004190. The one-word literal pool at 0x02004194
 * (0x00001e43, same `Value_XXXXXXXX` low-address family as elsewhere in
 * this overlay) is included per the usual pool rule, immediately
 * followed by the next owner's push {r5,lr} at 0x02004198, already this
 * overlay's row `0x02004198 | 1 call`, so the span is
 * 0x0200413c-0x02004198, 92 bytes.
 *
 * A flat setter sequence, no branches. Two distinct call sites happen to
 * target the same callee (`Func_0200a752`) with different arities.
 *
 * Raw callee naming.
 */

extern u8 Value_00001e43;

void Func_0808a018(void);
void Func_0808a210();
void Func_080000c0();
void Func_020048e8();
void Func_0808a138();
void Func_0808a170();
void Func_0200486c();
void Func_0808a1b8();

void Func_0200413c(void)
{
    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_020048e8(15, 1, 1);
    Func_0808a138(8, 1);
    Func_0808a170((s32)&Value_00001e43);
    Func_0200486c(8);
    Func_0808a1b8(8, 0xd000, 0x28);
    Func_020048e8(9, 15, 0);
}
