typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001ef0, 276 bytes: code 0x02001ef0-0x02001fff and
 * the pool word 0x00000309 at 0x02002000.
 *
 * The slot-17 walk beat, structurally the same as 0x02001150, 0x02001328 and
 * 0x02001de0: open the slot at (392, 104), publish the eight-argument piece
 * through Func_02000ae8, run the presentation, then walk west through
 * (376, 152), (328, 160) and (296, 160), waiting on Func_0808a148 after each.
 * The first waypoint keeps the opening height 0x60000; the later two drop to
 * 0x30000.
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

void Func_02001ef0(void)
{
    u8 *slot;

    slot = Func_0808a080(17);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(17, 1);
    Func_02000d90(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_0808a200(17, 1);
    Func_0808a150(17, 0, 0);
    Func_0808a010(20);
    Func_0808a130(17, 2);
    Func_0808a1f0(17, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    Func_02000d90(17, 376, 152, 0x60000);       /* 188 << 1 */
    Func_0808a148(0, 17, 0);
    Func_0808a010(10);

    Func_02000d90(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Func_0808a148(0, 17, 0);
    Func_0808a010(6);

    Func_02000d90(17, 296, 160, 0x30000);       /* 148 << 1 */
    Func_0808a148(0, 17, 0);
    Func_0808a010(6);

    Func_0808a200(0, 1);
    Func_0808a0f0(17, 0, 0);
    Func_0808a010(30);
    Func_080770c8(0x309);
    Func_0808a0f0(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
