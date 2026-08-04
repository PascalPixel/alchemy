typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x020015d0, 288 bytes: code 0x020015d0-0x020016e9, an
 * alignment halfword at 0x020016ea and the pool word 0x00000303 at
 * 0x020016ec.
 *
 * The slot-12 walk beat with four waypoints: open the slot at (392, 104),
 * publish the eight-argument piece through Func_02000ae8, run the
 * presentation, then walk south along a constant X of 424 through
 * Z = 120, 168, 208 and 232.
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

void Func_020015d0(void)
{
    u8 *slot;

    slot = Func_0808a080(12);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(12, 1);
    Func_02000d90(12, 392, 104, 0x70000);       /* 196 << 1, 224 << 11 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_0808a200(12, 1);
    Func_0808a150(12, 0, 0);
    Func_0808a010(20);
    Func_0808a130(12, 2);
    Func_0808a1f0(12, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    /* Four waypoints, X = 212 << 1 and height 192 << 10 throughout. */
    Func_02000d90(12, 424, 120, 0x30000);
    Func_0808a148(0, 12, 0);
    Func_0808a010(6);

    Func_02000d90(12, 424, 168, 0x30000);
    Func_0808a148(0, 12, 0);
    Func_0808a010(6);

    Func_02000d90(12, 424, 208, 0x30000);
    Func_0808a148(0, 12, 0);
    Func_0808a010(6);

    Func_02000d90(12, 424, 232, 0x30000);
    Func_0808a148(0, 12, 0);
    Func_0808a010(6);

    Func_0808a0f0(12, 0, 0);
    Func_080770c8(0x303);
    Func_0808a0f0(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
