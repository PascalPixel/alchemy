/* Draft, not exact: UiWindow_ClearInteriorTiles, 152 bytes. 2026-10-01
   (wave 1, slice 1): 795 (27 register-only, 4 operand, 6 reordered, 1
   inserted, 1 deleted), was 1490. The interior origin is the left and top
   parameters reused (left = x + 1; top = y + 1), as the reference's entry
   copies of them to r5 and r7 show, and that gives the reference's code
   after the call. Remaining: allocation before the call. The reference
   keeps right and bottom in r3 and r1 and gives width r6 and height r4
   (saved round the call); here right and bottom move to r5 and r7 and
   left takes r4. A 100-second permute from here reaches 145 (9
   register-only, 1 deleted) with temporaries no one would write. */
#include "RENDER_INPUT.H"

extern u8 *gWindowWork;
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

void UiWindow_ClearInteriorTiles(const struct RenderInput *window,
    u32 left, u32 top, u32 right, u32 bottom)
{
    u8 *work = gWindowWork;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u16 *tiles;
    u32 row;
    u32 column;

    x = left >> 3;
    y = top >> 3;
    right = (right + 7) >> 3;
    bottom = (bottom + 7) >> 3;
    y += window->y;
    x += window->x;
    right += window->x;
    bottom += window->y;
    left = x + 1;
    top = y + 1;
    width = right - x;
    height = bottom - y;
    UiWindow_ClearTileAttributesInRect(left, top, width, height);
    tiles = (u16 *)work + top * 32 + left;
    for (row = 0; row < height; row++) {
        for (column = 0; column < width; column++)
            *tiles++ = 0xf020;
        tiles += 32 - width;
    }
    work[0xea3] = 1;
}
