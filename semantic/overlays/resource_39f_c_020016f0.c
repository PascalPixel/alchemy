typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x020016f0, 296 bytes: code 0x020016f0-0x02001811, an
 * alignment halfword at 0x02001812 and the pool word 0x00033333 at
 * 0x02001814.
 *
 * The slot-13 walk beat with four waypoints: open the slot at (456, 104),
 * publish the eight-argument piece through Func_02000ae8, run the
 * presentation, then walk east along a constant Z of 136 through
 * X = 472, 504, 552 and 584.
 *
 * The heights are the interesting part.  The first waypoint uses 0x30000
 * (3.0 in 16.16), the second the pooled 0x33333 (3.2), and the last two
 * 0x38000 (3.5) - a short climb rather than a flat walk, which is why the
 * middle value needs a pool word instead of a movs/lsls pair.
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

void Func_020016f0(void)
{
    u8 *slot;

    slot = Func_0808a080(13);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(13, 1);
    Func_02000d90(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_0808a200(13, 1);
    Func_0808a150(13, 0, 0);
    Func_0808a010(20);
    Func_0808a130(13, 2);
    Func_0808a1f0(13, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    Func_02000d90(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Func_0808a148(0, 13, 0);
    Func_0808a010(6);

    Func_02000d90(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Func_0808a148(0, 13, 0);
    Func_0808a010(6);

    Func_02000d90(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Func_0808a148(0, 13, 0);
    Func_0808a010(6);

    Func_02000d90(13, 584, 136, 0x38000);       /* 146 << 2 */
    Func_0808a148(0, 13, 0);
    Func_0808a010(6);

    Func_0808a0f0(13, 0, 0);
    Func_080770c8(772);                         /* 193 << 2 */
    Func_0808a0f0(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
