/*
 * Draft: UiWindow_SetRectHighlight does not yet match; the stack arguments load later than in the ROM, the row start adds its terms the other way round, the loop counter decrements one slot late and the literal pool lands mid-function.
 * Links as recon/tla/raw/0804e0d0.s.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

struct UiWindowFrame {
    u8 unknown_00[0x0c];
    s16 x;                      /* 0x0c */
    s16 y;                      /* 0x0e */
};

/* Sets (alt bit 0 = 1) or clears palette bit 12 on every tile of a width x
   height rectangle inside a window's border, clipped to the screen, and
   marks the window tilemap dirty. */
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
