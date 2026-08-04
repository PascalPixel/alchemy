typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001d04, 220 bytes: code 0x02001d04-0x02001dcd, an
 * alignment halfword at 0x02001dce and the four pool words 0x00000103,
 * 0x00000307, 0x02000240 and 0x0000022b at 0x02001dd0-0x02001ddf.
 *
 * The slot-15 twin of 0x02001244: open the slot, place it at (472, 104),
 * publish an eight-argument piece through Func_02000ae8, run the presentation,
 * re-place the slot on the party's heading readings, and set the engine byte at
 * Data_02000240 + 0x22b to 3.  It differs from the slot-11 version by an extra
 * Func_0808a010(10) after the placement, a different Z bias, a different
 * notification tag, and by omitting the trailing Func_0808a0f0 call.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule,
 * and resource_39f_c_02001244.c for the Data_02000240 note.
 *
 * The epilogue is `add sp, #16 / pop {r5, r6} / pop {r0} / bx r0`, so the owner
 * is void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue is only
 * `push {r5, r6, lr}`.  It carries the constant 0 into three stack argument
 * slots; nothing observable depends on the register choice.
 */

extern s16 Data_02000240[];

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
void Func_0808a250();
void Func_0808a020();

void Func_02001d04(void)
{
    u8 *slot;

    slot = Func_0808a080(15);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(15, 0);
    Func_02000d90(15, 472, 104, 0x80000);   /* 236 << 1, 128 << 12 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x80000,
                  0, 0, 0, 1, 0);

    Func_0808a200(15, 1);
    Func_0808a150(15, 0, 0);
    Func_0808a010(30);
    Func_0808a130(15, 2);
    Func_0808a1e8(15, 0x103, 0);
    Func_080f9010(147);
    Func_0808a010(60);

    Func_02000d90(15,
                  *(s16 *)(Func_0808a080(0) + 10),
                  *(s16 *)(Func_0808a080(0) + 18),
                  0x60000);                          /* 192 << 11 */

    Func_0808a010(10);
    Func_080770c8(0x307);

    ((u8 *)Data_02000240)[0x22b] = 3;

    Func_0808a250(53, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
