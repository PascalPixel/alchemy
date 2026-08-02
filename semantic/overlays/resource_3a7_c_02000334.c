/*
 * resource_3a7 @ 0x02000334 (52 bytes: 48 code and one pool word).
 *
 * Returns whether actor slot 9 occupies tile (15, 54).  Negative fixed-point
 * coordinates are biased before the arithmetic shift, reproducing truncation
 * toward zero rather than flooring them.
 */
typedef signed int s32;
typedef unsigned char u8;

u8 *Func_0808a080(s32 slot);

s32 Func_02000334(void)
{
    u8 *actor = Func_0808a080(9);
    s32 y = *(s32 *)(actor + 16);
    s32 x = *(s32 *)(actor + 8);

    if (y < 0)
        y += 0xfffff;
    y >>= 20;

    if (x < 0)
        x += 0xfffff;
    x >>= 20;

    return x == 15 && y == 54;
}
