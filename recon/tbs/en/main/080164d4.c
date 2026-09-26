/* NONMATCHING: complete 152-byte listing through 0801656c, including the
 * final alignment halfword; candidate 150 bytes, 50 differing halfwords /
 * 48 aligned edits (2026-09-26). Separate coordinate conversion and origin
 * addition preserve all four absolute coordinates. Reusing left/top inputs
 * gives 152/57/53 with an extra saved sl; an unused-result call cast only
 * changes argument setup (152/57/52), so it is rejected. Separate signed
 * x/y locals restore the single r8 save and the reference's pool/epilogue
 * offsets. Retain that typed model and the actual void callee interface.
 * Remaining: coordinate setup, width and x/y register lifetimes, then tile
 * cursor/row/column allocation. Three bounded hypotheses; no credit. */
#include "RENDER_INPUT.H"

extern u8 *Data_03001e8c;
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

void Func_080164d4(const struct RenderInput *window,
    u32 left, u32 top, u32 right, u32 bottom)
{
    u8 *work;
    u16 *tiles;
    s32 x;
    s32 y;
    u32 width;
    u32 height;
    u32 row;
    u32 column;

    work = Data_03001e8c;
    x = left >> 3;
    y = top >> 3;
    right = (right + 7) >> 3;
    bottom = (bottom + 7) >> 3;
    x += window->x;
    y += window->y;
    right += window->x;
    bottom += window->y;
    width = right - x;
    height = bottom - y;
    x++;
    y++;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    tiles = (u16 *)work + y * 32 + x;
    for (row = 0; row < height; row++) {
        for (column = 0; column < width; column++)
            *tiles++ = 0xf020;
        tiles += 32 - width;
    }
    work[0xea3] = 1;
}
