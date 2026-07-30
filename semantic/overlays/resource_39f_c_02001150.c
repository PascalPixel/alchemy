typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001150, 244 bytes (0x02001150-0x02001243), no
 * literal pool.
 *
 * A walk beat for slot 10: open the slot at (88, 120), publish the
 * eight-argument piece through Func_02000ae8, run the presentation, then step
 * the slot along three waypoints - (88, 152), (120, 192), (120, 240) - waiting
 * on Func_0808a148 after each, and close with two Func_0808a0f0 notifications.
 *
 * The same beat for slot 12 is at 0x02001328.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * This owner contains a good illustration of why the rule matters: the branches
 * at 0x02001218 and 0x02001220 both decode pc-relatively to 0x02003fb6, yet
 * they are different callees - the stored displacements are 0x2d9a and 0x2d92,
 * so the real targets are veneers 0x02002d9c (Func_0808a010) and 0x02002d94
 * (Func_080770c8).
 *
 * The epilogue is `add sp, #16 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue is only
 * `push {r5, lr}`.  It carries the constant 0 into three stack argument slots.
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

void Func_02001150(void)
{
    u8 *slot;

    slot = Func_0808a080(10);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(10, 1);
    Func_02000d90(10, 88, 120, 0x60000);        /* 192 << 11 */

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_0808a200(10, 1);
    Func_0808a150(10, 0, 0);
    Func_0808a010(20);
    Func_0808a130(10, 2);
    Func_0808a1f0(10, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    Func_02000d90(10, 88, 152, 0x30000);
    Func_0808a148(0, 10, 0);
    Func_0808a010(10);

    Func_02000d90(10, 120, 192, 0x30000);
    Func_0808a148(0, 10, 0);
    Func_0808a010(10);

    Func_02000d90(10, 120, 240, 0x30000);
    Func_0808a148(0, 10, 0);
    Func_0808a010(10);

    Func_080770c8(768);                         /* 192 << 2 */
    Func_0808a0f0(13, 0, 0);
    Func_0808a0f0(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
