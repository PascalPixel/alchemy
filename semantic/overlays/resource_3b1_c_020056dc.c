typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x020056dc (164 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` plus `r8` spill at 0x020056dc, epilogue `pop {r3} / mov
 * r8,r3 / pop {r5,r6} / pop {r0} / bx r0` at 0x0200576c-0x02005774. The
 * two-word pool at 0x02005778-0x0200577c (0x0200e840, 0x0200e8e4, the
 * same handle/pointer values used at 0x02004198 and 0x02005068 earlier
 * in this overlay) is included per the usual pool rule, immediately
 * followed by the next owner's push {r5,r6,lr} at 0x02005780, already
 * this overlay's row `0x02005780 | 1 call`, so the span is
 * 0x020056dc-0x02005780, 164 bytes.
 *
 * A flat setter sequence, no branches. The final call
 * (`Func_0200bc3a`, same target as an earlier call in this function)
 * carries no explicit argument setup in the reference.
 *
 * Raw callee naming.
 */

s32 Func_02004fa8();
void Func_0808a018(void);
void Func_020048e8();
void Func_020037b4(void);
void Func_0808a100();
void Func_0808a098();
void Func_0808a088();
void Func_0808a158();
void Func_020050e4(void);
void Func_0808a020(void);

void Func_020056dc(void)
{
    s32 handle = Func_02004fa8(0, 0);
    s32 other = Func_02004fa8(1, 0);

    Func_0808a018();
    Func_020048e8(24, 1, 0);
    Func_020048e8(25, 3, 0);
    Func_020037b4();
    Func_020048e8(19, handle, other);
    Func_0808a100(10, 6);
    Func_0808a098(handle, 0x0200e840);
    Func_0808a088(11);
    Func_0808a098(other, 0x0200e840);
    Func_0808a088(12);
    Func_0808a098(0x24, 0x0200e8e4);
    Func_0808a098(0x25, 0x0200e8e4);
    Func_0808a158(0x24, 3);
    Func_0808a158(0x25, 3);
    Func_020050e4();
    Func_0808a020();
}
