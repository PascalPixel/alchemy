/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * This draft had modelled the callee as a fourth ARGUMENT (and as an
 * `extern` data symbol at 0x03001388).  It was never data and never an
 * argument: it is the call target.
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

u16 *Func_08002f40(s32);

void Func_080e46f0(s32 index)
{
    u16 colors[64];
    u16 *palette = (u16 *)0x05000000;
    u16 *palette_cursor;
    s32 i;

    ((WordCopy)0x03001388)(colors, Func_08002f40(index), sizeof(colors));
    colors[0] = 0;

    palette_cursor = palette;
    i = 0;
    do {
        u32 current = *palette_cursor;
        u32 target = colors[i];
        s32 red = current & 0x1f;
        s32 green = (current >> 5) & 0x1f;
        s32 blue = (current >> 10) & 0x1f;
        s32 target_red = target & 0x1f;
        s32 target_green = (target >> 5) & 0x1f;
        s32 target_blue = (target >> 10) & 0x1f;

        if (red < target_red)
            red++;
        else if (red > target_red)
            red--;
        if (green < target_green)
            green++;
        else if (green > target_green)
            green--;
        if (blue < target_blue)
            blue++;
        else if (blue > target_blue)
            blue--;

        colors[i] = red | (green << 5) | (blue << 10);
        i++;
        palette_cursor++;
    } while (i != 64);

    ((WordCopy)0x03001388)(palette, colors, sizeof(colors));
}
