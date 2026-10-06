#include "TYPES.H"
#include "RAM_BUFFER.H"

struct UiWindowFrame {
    u8 unknown_00[0x0c];
    s16 x;
    s16 y;
};

void UiWindow_SetRectHighlight(struct UiWindowFrame *window, s32 x, s32 y,
    s32 width, s32 height, s32 alt)
{
    u8 *tiles = Ram_HeapSlots->window_tiles;
    u8 *row;
    u16 *p;
    s32 col;

    x = x + window->x + 1;
    y = y + window->y + 1;
    alt &= 1;
    alt <<= 12;
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
        row = (y << 6) + (u8 *)((u16 *)tiles + x);
        do {
            p = (u16 *)(row + 8);
            col = width;
            if (col != 0) {
                do {
                    u32 tile;
                    col--;
                    tile = *p;
                    *p = (tile & ~0x1000) | alt;
                    p++;
                } while (col != 0);
            }
            height--;
            row += 64;
        } while (height != 0);
        tiles[3] = 1;
    }
}
