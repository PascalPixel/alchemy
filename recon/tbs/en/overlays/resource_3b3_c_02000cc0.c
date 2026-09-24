/* NONMATCHING: 192 of 184 bytes, 84 differing halfwords (2026-09-24).
 * Hand-written from the disassembly: copies a width x height block of the
 * 128-wide cell map at 0x02010000 into the BG screen block at 0x06002800 (two
 * words per cell from the table at 0x02020000). The loop shape matches;
 * register allocation does not: the reference keeps top in r4, src in r5,
 * width in sl, block << 4 in r9 and 128 - width in fp and spills only bottom
 * and the row base, here two more values spill. */
#include "TYPES.H"

void Func_02000cc0(s32 x, s32 y, s32 width, s32 height, s32 block, s32 left, s32 top)
{
    u32 *src;
    s32 bottom;
    s32 col;
    s32 base;
    u32 cell;

    src = (u32 *)0x02010000 + (y << 7) + x;
    bottom = top + height;
    for (; top < bottom; top++) {
        for (col = left; col < left + width; col++) {
            base = ((top & 15) + (block << 4)) << 5;
            cell = *src++;
            ((u32 *)0x06002800)[base + (col & 15)] = ((u32 *)0x02020000)[(cell & 0xfff) * 2];
            ((u32 *)0x06002840)[base + (col & 15)] = ((u32 *)0x02020004)[(cell & 0xfff) * 2];
        }
        src += 128 - width;
    }
}
