/* Not-yet-C: complete 288-byte owner, separated from the former 1644-byte
 * bundle. RenderInput fixes the old draft's swapped x/y fields; first column
 * starts at index 1 unless flags select bias 5, and the bottom cap is 0xf019.
 * Corrected model: 280/288 bytes, 133 halfwords / 77 aligned edits.
 * Address grouping reproduces the row/column sums. A one-pass bottom-cap
 * store still merges the tail (75 edits) and was not retained. Remaining:
 * reference spills bias to a 4-byte frame, retains the window in r6 and
 * chooses top+1 for the bottom constant; this candidate keeps bias in fp.
 * Stop after the model and two structural trials; inspect allocator/CSE. */
#include "TYPES.H"
#include "RENDER_INPUT.H"

extern u8 *Data_03001e8c;
extern const s8 Data_080371c4[];

void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags)
{
    u8 *base = Data_03001e8c;
    u32 max = window->width - 1;
    u32 rows = window->height;
    s32 bias = 0;
    s32 first = 1;
    s32 i;

    if ((flags & 1) == 0)
        flags &= ~2;
    if (flags & 2) {
        bias = 5;
        first = 0;
    }
    for (i = first; Data_080371c4[i] >= 0; i++) {
        u32 col = Data_080371c4[i] + bias;
        if (col < max) {
            u32 row;
            for (row = 0; row != rows; row++) {
                u16 *dest = (u16 *)((((window->y + row) << 5) +
                    (window->x + col)) * 2 + (u32)base);
                if (row == 0)
                    *dest = 0xf018;
                else if (row == rows - 1)
                    *dest = 0xf019;
                else
                    *dest = 0xf00f;
            }
        }
    }
    if (base[0xea5] != 0) {
        u16 *dest = (u16 *)((u32)base + ((window->y + window->height) << 6) +
            (window->x << 1) - 64);
        u32 col;
        *dest++ = 0xf080;
        for (col = 1; col < max; col++)
            *dest++ = 0xf081;
        *dest = 0xf082;
    }
    base[0xea3] = 1;
}
