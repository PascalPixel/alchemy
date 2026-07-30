typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001328, 248 bytes: code 0x02001328-0x0200141b and
 * the pool word 0x00000302 at 0x0200141c.
 *
 * The slot-12 twin of 0x02001150: open the slot at (536, 344), publish the
 * eight-argument piece through Func_02000ae8, run the presentation, then walk
 * the slot east through (584, 344), (632, 344) and (680, 344) - a constant Z of
 * 344 and a 48-unit stride - waiting on Func_0808a148 after each.
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
/* Four register arguments plus four stack words. */
void Func_02000ae8();
void Func_0808a200();
void Func_0808a150();
void Func_0808a010();
void Func_0808a130();
void Func_0808a1f0();
void Func_0808a148();
void Func_080770c8();
void Func_0808a0f0();
void Func_0808a020();

void Func_02001328(void)
{
    u8 *slot;

    slot = Func_0808a080(12);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(12, 1);
    Func_02000d90(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Func_0808a200(12, 1);
    Func_0808a150(12, 0, 0);
    Func_0808a010(20);
    Func_0808a130(12, 2);
    Func_0808a1f0(12, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    Func_02000d90(12, 584, 344, 0x30000);
    Func_0808a148(0, 12, 0);
    Func_0808a010(6);

    Func_02000d90(12, 632, 344, 0x30000);
    Func_0808a148(0, 12, 0);
    Func_0808a010(6);

    Func_02000d90(12, 680, 344, 0x30000);
    Func_0808a148(0, 12, 0);
    Func_0808a010(6);

    Func_080770c8(0x302);
    Func_0808a0f0(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
