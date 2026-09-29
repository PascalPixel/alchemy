/* 2026-09-29 alchemy permute: score 1490 to 1015 on the permuter's scorer
   (0 is exact); remaining 27 register-only, 5 operand, 9 reordered, 2
   deleted. Kept rewrites: 12x reorder independent statements, 6x introduce
   a temporary, 4x reorder local declarations, 3x remove a temporary, 3x
   toggle register, 2x change loop form, 2x split or join a compound
   assignment, 1x add a same-width cast, 1x drop a same-width cast.
   FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
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
    u32 height;
    s32 y;
    u32 width;
    u32 row;
    u32 tmp;
    register u32 column;
    u32 tmp4;
    u16 tmp2;

    y = top >> 3;
    tmp4 = right + 7;
    tmp = (bottom + 7) >> 3;
    work = Data_03001e8c;
    bottom = tmp;
    x = left >> 3;
    right = tmp4 >> 3;
    y += window->y;
    tmp2 = window->x;
    x += tmp2;
    bottom = bottom + window->y;
    right += window->x;
    width = right - x;
    height = bottom - y;
    x++;
    ++y;
    tiles = (u16 *)work + y * 32 + x;
    row = 0;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    if (row < height) {
        do {
            column = 0;
            if (column < width) {
                do {
                    *tiles++ = 0xf020;
                    column++;
                } while (column < width);
            }
            tiles += 32 - width;
            row++;
        } while (row < height);
    }
    work[0xea3] = 1;
}
