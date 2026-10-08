#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MAP.H"
#include "IO_REG.H"

void Map_DrawCellPanel(s32 x, s32 y, s32 width, s32 height, s32 plane, s32 destX, s32 destY)
{
    u32 *cell = (u32 *)Ram_MapCellBuffer + (y * 128 + x);
    s32 row;
    s32 col;
    s32 end;

    for (row = destY; row < destY + height; row++) {
        col = destX;
        if (col < destX + width) do {
            u16 tile = 0xfff & *cell++;
            u16 offset = tile * 8;
            s32 index = (plane * 16 + (row & 15)) * 32 + (col & 15);
            u32 *src;
            u32 *dst;

            dst = (u32 *)BG_SCREEN_BLOCK(5) + index;
            src = (u32 *)(Ram_MapBlocks + offset);
            *dst = *src;
            src = (u32 *)(Ram_MapBlocks + 4 + offset);
            dst = (u32 *)(BG_SCREEN_BLOCK(5) + 64) + index;
            *dst = *src;

        /* FAKEMATCH: needed to match: end is a forced temporary for the bound's evaluation order. */
        } while (end = destX + width, ++col < end);
        cell += 128 - width;
    }
}
