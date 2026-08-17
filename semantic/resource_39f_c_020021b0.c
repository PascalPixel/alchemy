/*
 * BYTE-EXACT 2026-08-07 with the new -fthumb-small-shift-before-immediates.
 * Four argument sheets here write a small split-constant shift (`lsls rN, #1')
 * ahead of the sheet's plain `movs r0/r1' immediates, and a second split
 * constant's shift follows its partner directly.  The rule is the mirror of
 * -fthumb-arg-before-final-shift, which moves an immediate ahead of a shift of
 * seven or more; the sheet at 0x020021c0 shifts by 16 and 17 and is therefore
 * left alone, as the reference leaves it.
 */
#include "types.h"

/*
 * resource_39f owner at 0x020021b0, 272 bytes: code 0x020021b0-0x020022b9, an
 * alignment halfword at 0x020022ba and the pool word 0x0000089d at
 * 0x020022bc.
 *
 * The slot-18 walk beat.  It differs from the other members of the family by
 * an opening Func_0808a0f0 that carries a 16.16 coordinate pair rather than a
 * pair of zeros, and by using Func_0808a1b8/Func_0808a138 in place of
 * Func_0808a150/Func_0808a130.  The slot then walks south along a constant X of
 * 136 through Z = 440, 472 and 504.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `add sp, #16 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue is only
 * `push {r5, lr}`.  It carries the constant 0 into three stack argument slots.
 */







/* Four register arguments plus four stack words. */









extern u8 * Func_02004f74();
extern void Func_02004f62();
extern void Func_02004fc0();
extern void Func_0200307c();
extern void Func_02002f74();
extern void Func_02004f86();
extern void Func_02002cf0();
extern void Func_02005048();
extern void Func_0200506a();
extern void Func_02005032();
extern void Func_02005082();
extern void Func_02002fce();
extern void Func_0200505c();
extern void Func_02004fec();
extern void Func_02002ff0();
extern void Func_0200507e();
extern void Func_0200500c();
extern void Func_0200300e();
extern void Func_0200509c();
extern void Func_0200502a();
extern void Func_020050ea();
extern void Func_02005094();
extern void Func_02005042();
extern void Func_02005040();
void Func_020021b0(void)
{
    u8 *slot;

    slot = Func_02004f74(18);

    /* r0 still holds the record returned above. */
    Func_02004f62();

    /* 136 << 16 and 180 << 17: the same two numbers the walk uses, in 16.16. */
    Func_02004fc0(18, 136 << 16, 360 << 16);

    Func_0200307c(18, 1);
    Func_02002f74(18, 136, 408, 0x80000);       /* 204 << 1, 128 << 12 */
    Func_02004f86(10);

    Func_02002cf0(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02005048(18, 0xc000, 40);              /* 192 << 8 */
    Func_0200506a(18, 258);                     /* 129 << 1 */
    Func_02005032(18, 2);
    Func_02005082(18, 1);

    Func_02002fce(18, 136, 440, 0x60000);       /* 220 << 1, 192 << 11 */
    Func_0200505c(0, 18, 0);
    Func_02004fec(10);

    Func_02002ff0(18, 136, 472, 0x30000);       /* 236 << 1, 192 << 10 */
    Func_0200507e(0, 18, 0);
    Func_0200500c(6);

    Func_0200300e(18, 136, 504, 0x30000);       /* 252 << 1 */
    Func_0200509c(0, 18, 0);
    Func_0200502a(6);

    Func_020050ea(0, 1);
    Func_02005094(18, 0, 0);
    Func_02005042(60);
    Func_02005040(0x89d);

    /* Common exit; no argument registers are set. */
    Func_0200505c();
}
