typedef unsigned char u8;
typedef signed int s32;

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
 * Raw callee naming.
 */

extern u8 Value_0000092a;

s32 Func_02004fa8();
void Func_0808a018(void);
void Func_020048e8();
void Func_020037d8();
void Func_0808a0f0();
void Func_020052f4(void);
void Func_080770c8();
void Func_0808a020(void);

void Func_02005780(void)
{
    s32 handle = Func_02004fa8(0, 0);
    s32 other = Func_02004fa8(1, 0);

    Func_0808a018();
    Func_020048e8(24, 1, 0);
    Func_020048e8(25, 0, 0);
    Func_020037d8(0);
    Func_020048e8(19, handle, other);
    Func_0808a0f0(11, 0, 0);
    Func_0808a0f0(12, 0, 0);
    Func_020052f4();
    Func_080770c8((s32)&Value_0000092a);
    Func_0808a020();
}
