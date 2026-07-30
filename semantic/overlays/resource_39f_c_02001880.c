typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001880, 312 bytes: code 0x02001880-0x020019b1, an
 * alignment halfword at 0x020019b2 and the pool word 0x00000306 at
 * 0x020019b4.
 *
 * The slot-14 walk beat with four waypoints: open the slot at (392, 504),
 * publish the eight-argument piece through Func_02000ae8, run the
 * presentation, then walk west along a constant Z of 528 through
 * X = 360, 328, 288 and 256.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `add sp, #16 / pop {r5, r6} / pop {r0} / bx r0`, so the owner
 * is void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue is only
 * `push {r5, r6, lr}`.  It carries the constant 0 into three stack argument
 * slots.
 */

u8 *Func_0808a080();
void Func_0808a018();
void Func_02000ea8();
void Func_02000d90();
void Func_0808a010();
/* Four register arguments plus four stack words. */
void Func_02000ae8();
void Func_0808a200();
void Func_0808a150();
void Func_0808a130();
void Func_0808a1f0();
void Func_0808a148();
void Func_0808a0f0();
void Func_080770c8();
void Func_0808a020();

void Func_02001880(void)
{
    u8 *slot;

    slot = Func_0808a080(14);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(14, 1);
    Func_02000d90(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_0808a200(14, 1);
    Func_0808a150(14, 0, 0);
    Func_0808a010(20);
    Func_0808a130(14, 2);
    Func_0808a1f0(14, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    Func_02000d90(14, 360, 528, 0x30000);
    Func_0808a148(0, 14, 0);
    Func_0808a010(6);

    Func_02000d90(14, 328, 528, 0x30000);
    Func_0808a148(0, 14, 0);
    Func_0808a010(6);

    Func_02000d90(14, 288, 528, 0x30000);
    Func_0808a148(0, 14, 0);
    Func_0808a010(6);

    Func_02000d90(14, 256, 528, 0x30000);
    Func_0808a148(0, 14, 0);
    Func_0808a010(6);

    Func_0808a200(0, 1);
    Func_0808a0f0(14, 0, 0);
    Func_0808a010(30);
    Func_080770c8(0x306);
    Func_0808a0f0(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
