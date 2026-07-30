typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001de0, 272 bytes (0x02001de0-0x02001eef), no
 * literal pool.
 *
 * The slot-16 walk beat, the same construct as 0x02001150 and 0x02001328: open
 * the slot at (456, 152), publish the eight-argument piece through
 * Func_02000ae8, run the presentation, then walk through (448, 192),
 * (424, 208) and (424, 224), waiting on Func_0808a148 after each, and close
 * with a notification pair.
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

void Func_02001de0(void)
{
    u8 *slot;

    slot = Func_0808a080(16);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(16, 1);
    Func_02000d90(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_0808a200(16, 1);
    Func_0808a150(16, 0, 0);
    Func_0808a010(20);
    Func_0808a130(16, 2);
    Func_0808a1f0(16, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    Func_02000d90(16, 448, 192, 0x30000);       /* 224 << 1 */
    Func_0808a148(0, 16, 0);
    Func_0808a010(6);

    Func_02000d90(16, 424, 208, 0x30000);       /* 212 << 1 */
    Func_0808a148(0, 16, 0);
    Func_0808a010(6);

    Func_02000d90(16, 424, 224, 0x30000);
    Func_0808a148(0, 16, 0);
    Func_0808a010(6);

    Func_0808a200(0, 1);
    Func_0808a0f0(16, 0, 0);
    Func_0808a010(30);
    Func_080770c8(776);                         /* 194 << 2 */
    Func_0808a0f0(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
