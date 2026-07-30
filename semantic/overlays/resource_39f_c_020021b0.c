typedef signed int s32;
typedef unsigned char u8;

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

u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a0f0();
void Func_02000ea8();
void Func_02000d90();
void Func_0808a010();
/* Four register arguments plus four stack words. */
void Func_02000ae8();
void Func_0808a1b8();
void Func_0808a1f0();
void Func_0808a138();
void Func_0808a200();
void Func_0808a148();
void Func_080770c8();
void Func_0808a020();

void Func_020021b0(void)
{
    u8 *slot;

    slot = Func_0808a080(18);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    /* 136 << 16 and 180 << 17: the same two numbers the walk uses, in 16.16. */
    Func_0808a0f0(18, 136 << 16, 360 << 16);

    Func_02000ea8(18, 1);
    Func_02000d90(18, 136, 408, 0x80000);       /* 204 << 1, 128 << 12 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_0808a1b8(18, 0xc000, 40);              /* 192 << 8 */
    Func_0808a1f0(18, 258);                     /* 129 << 1 */
    Func_0808a138(18, 2);
    Func_0808a200(18, 1);

    Func_02000d90(18, 136, 440, 0x60000);       /* 220 << 1, 192 << 11 */
    Func_0808a148(0, 18, 0);
    Func_0808a010(10);

    Func_02000d90(18, 136, 472, 0x30000);       /* 236 << 1, 192 << 10 */
    Func_0808a148(0, 18, 0);
    Func_0808a010(6);

    Func_02000d90(18, 136, 504, 0x30000);       /* 252 << 1 */
    Func_0808a148(0, 18, 0);
    Func_0808a010(6);

    Func_0808a200(0, 1);
    Func_0808a0f0(18, 0, 0);
    Func_0808a010(60);
    Func_080770c8(0x89d);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
