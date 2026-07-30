typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02002078, 312 bytes: code 0x02002078-0x020021a9, an
 * alignment halfword at 0x020021aa and the pool word 0x0000030b at
 * 0x020021ac.
 *
 * The slot-18 walk beat with four waypoints: open the slot at (712, 536),
 * publish the eight-argument piece through Func_02000ae8, run the
 * presentation, then walk south through (712, 568), (712, 600), (736, 640) and
 * (736, 704).  The X literal is bumped by 24 in place before the third
 * waypoint, which is why the last two share it.
 *
 * The first waypoint keeps the opening height 0x60000, held in r8 across the
 * presentation; the rest use 0x30000.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `add sp, #16 / pop {r3} / ... / pop {r0} / bx r0`, so the
 * owner is void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue saves
 * only r5, r6, lr and r8.  It carries the constant 0 into three stack argument
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

void Func_02002078(void)
{
    u8 *slot;

    slot = Func_0808a080(18);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_02000ea8(18, 1);
    Func_02000d90(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Func_0808a010(10);

    Func_02000ae8(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_0808a200(18, 1);
    Func_0808a150(18, 0, 0);
    Func_0808a010(20);
    Func_0808a130(18, 2);
    Func_0808a1f0(18, 258);                     /* 129 << 1 */
    Func_0808a010(60);

    Func_02000d90(18, 712, 568, 0x60000);       /* 142 << 2 */
    Func_0808a148(0, 18, 0);
    Func_0808a010(10);

    Func_02000d90(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Func_0808a148(0, 18, 0);
    Func_0808a010(6);

    Func_02000d90(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Func_0808a148(0, 18, 0);
    Func_0808a010(6);

    Func_02000d90(18, 736, 704, 0x30000);       /* 176 << 2 */
    Func_0808a148(0, 18, 0);
    Func_0808a010(6);

    Func_0808a200(0, 1);
    Func_0808a0f0(18, 0, 0);
    Func_0808a010(30);
    Func_080770c8(0x30b);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
