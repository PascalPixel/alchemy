typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001244, 228 bytes: code 0x02001244-0x02001315, an
 * alignment halfword at 0x02001316 and the four pool words 0x00000103,
 * 0x00000301, 0x02000240 and 0x0000022b at 0x02001318-0x02001327.
 *
 * A full cutscene beat for slot 11: open the slot, place it at (408, 456),
 * publish an eight-argument piece through the overlay's Func_02000ae8, run the
 * presentation, then re-place the slot on the party's current heading readings
 * and set the engine byte at Data_02000240 + 0x22b to 3.
 *
 * The twin at 0x02001d04 is the same beat for slot 15.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * Data_02000240 is a genuine EWRAM global, not overlay data: this module is
 * linked at 0x02008000, so an address below that base cannot be in-image.  The
 * byte-exact sources in assets/code use the same symbol.
 *
 * The epilogue is `add sp, #16 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue is only
 * `push {r5, lr}`.  It carries the constant 0 into three stack argument slots;
 * nothing observable depends on the register choice.
 */

extern s16 Data_02000240[];

/* Returns a slot record; +8/+12/+16 are its position and +10/+18 are the
 * signed halfwords read from slot 0. */
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
void Func_0808a1e8();
void Func_080f9010();
void Func_080770c8();
void Func_0808a0f0();
void Func_0808a250();
void Func_0808a020();

void Func_02001244(void)
{
    u8 *slot;

    slot = Func_0808a080(11);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(11, 0);
    Func_02000d90(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_0808a200(11, 1);
    Func_0808a150(11, 0, 0);
    Func_0808a010(30);
    Func_0808a130(11, 2);
    Func_0808a1e8(11, 0x103, 0);
    Func_080f9010(147);
    Func_0808a010(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    Func_02000d90(11,
                  *(s16 *)(Func_0808a080(0) + 10),
                  *(s16 *)(Func_0808a080(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Func_0808a010(10);
    Func_080770c8(0x301);
    Func_0808a0f0(14, 0, 0);

    ((u8 *)Data_02000240)[0x22b] = 3;

    Func_0808a250(53, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
