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
 * A flat setter sequence, no branches. Per-site call veneers (raw asm
 * confirms each callee slot uses its own local stub, distinct from the
 * generic main-image symbol name); the final call reuses the exact same
 * local veneer (`sub_0200bc3a`) as an earlier call in this function and
 * carries no explicit argument setup in the reference (whatever is left
 * in r0/r1 from the prior call passes through), so it is called here
 * with no arguments to match.
 */

s32 Func_0200a690();
s32 Func_0200a69a();
void Func_0200bbc0(void);
void Func_02009fea();
void Func_02009ff4();
void Func_02008ec4(void);
void Func_0200a002();
void Func_0200bc72();
void Func_0200bc2c();
void Func_0200bc22();
void Func_0200bc3a();
void Func_0200bc30();
void Func_0200bc4a();
void Func_0200bc52();
void Func_0200bcf2();
void Func_0200bcfa();
void Func_0200a84a(void);

void Func_020056dc(void)
{
    s32 handle = Func_0200a690(0, 0);
    s32 other = Func_0200a69a(1, 0);

    Func_0200bbc0();
    Func_02009fea(24, 1, 0);
    Func_02009ff4(25, 3, 0);
    Func_02008ec4();
    Func_0200a002(19, handle, other);
    Func_0200bc72(10, 6);
    Func_0200bc2c(handle, 0x0200e840);
    Func_0200bc22(11);
    Func_0200bc3a(other, 0x0200e840);
    Func_0200bc30(12);
    Func_0200bc4a(0x24, 0x0200e8e4);
    Func_0200bc52(0x25, 0x0200e8e4);
    Func_0200bcf2(0x24, 3);
    Func_0200bcfa(0x25, 3);
    Func_0200a84a();
    Func_0200bc3a();
}
