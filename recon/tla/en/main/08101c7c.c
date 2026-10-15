#include "types.h"
#include "ram_buffer.h"

struct WindowTiles {
    u8 unk00[3];
    u8 dirty;
    u8 unk04[4];
    u16 cells[32][32];
};

void Ui_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    struct WindowTiles *tiles = (struct WindowTiles *)Ram_HeapSlots->windowTiles;

    palette <<= 12;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }

    if (width > 0 && height > 0) {
        do {
            u16 *p = &tiles->cells[y][x];
            s32 col = width;

            if (col != 0) {
                do {
                    u32 tile = *p;

                    if (((tile >> 12) & 15) == 15) {
                        tile &= ~0xf000;
                        tile |= palette;
                        *p = tile;
                    }
                    col--;
                    p++;
                } while (col != 0);
            }
            tiles->dirty |= 2 << ((u32)y >> 2);
            
            height--;
            y++;
        } while (height != 0);
    }
}
