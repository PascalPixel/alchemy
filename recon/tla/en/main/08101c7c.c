/* Near miss: differ 6 (90/90 instructions), two ARM9 scheduling swaps.
   The listing loads the window tiles (ldr r3, [r3, #60]) before copying
   the palette out of r10 to shift it, and marks the row dirty (orrs, strb)
   before counting it down (subs r5) and stepping the row (movs r3, #64).
   Ui_SetRectHighlight's barrier after the dirty mark fixes the second but
   swaps width and its temporary between r6 and r7 (26); barriers before
   the shift or between height-- and y++ do the same. The mask must stay a
   32-bit tile op (tile &= ~0xf000; tile |= palette): writing the store
   directly narrows ~0xf000 to 0x0fff built inline. Loop and clip shapes
   (--height, y++ first, width + x, early return) leave 6. It would go after
   UiPanel_SetRectHighlight in PANEL_HIGHLIGHT.C with 08101c40, and
   08101d34 (UiPanel_SetRectHighlight's twin calling it) after it. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

struct WindowTiles {
    u8 unknown_00[3];
    u8 dirty;
    u8 unknown_04[4];
    u16 cells[32][32];
};

/* Gives every tile of palette 15 in a width x height rectangle of the
   window tilemap the given palette, clipped to the screen, and marks each
   touched group of four rows dirty. */
void Ui_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    struct WindowTiles *tiles = (struct WindowTiles *)Ram_HeapSlots->window_tiles;

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
