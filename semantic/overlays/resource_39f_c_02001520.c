typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001520, 176 bytes: code 0x02001520-0x020015c9, an
 * alignment halfword at 0x020015ca and the pool word 0xfffe0000 at 0x020015cc.
 *
 * Repaints slot 11's collision footprint: it marks the actor's own tile 0xff
 * and clears the four orthogonally adjacent tiles, then - when the actor stands
 * exactly on tile (36, 24) - parks the record by clearing its mode byte and
 * writing -2.0 (0xfffe0000 in 16.16) into the two words at +12 and +20.
 *
 * The twin at 0x02001b84 is the same routine for slot 13 and tile (45, 6).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * The five painting calls resolve to 0x02000244, for which
 * assets/code/resource_39f_c_02000244.c is already byte-exact.  That source's
 * interface is `(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)` -
 * four register arguments and two stack words - and every call site here sets
 * exactly r0-r3 plus sp+0 and sp+4.  The ABI agreement is an independent check
 * on the call-resolution rule.
 *
 * The epilogue is `add sp, #8 / pop {r3} / ... / pop {r0} / bx r0`, so the
 * owner is void.
 */

void Func_0808a018();
/* Returns slot records; the two calls read different fields of the result. */
s32 *Func_0808a080();
/* Byte-exact: (layer, x, z, width, height, value). */
s32 Func_02000244();
void Func_0808a020();

void Func_02001520(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Func_0808a018();

    /* Both coordinates are 16.16 fixed point reduced to whole tiles with
     * `asrs #20`, i.e. 16 fractional bits plus a 16-unit tile pitch. */
    x = Func_0808a080(11)[2] >> 20;
    z = Func_0808a080(11)[4] >> 20;

    Func_02000244(2, x, z, 1, 1, 0xff);
    Func_02000244(2, x + 1, z, 1, 1, 0);
    Func_02000244(2, x - 1, z, 1, 1, 0);
    Func_02000244(2, x, z + 1, 1, 1, 0);
    Func_02000244(2, x, z - 1, 1, 1, 0);

    if (x == 36 && z == 24) {
        u8 *record = (u8 *)Func_0808a080(11);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
