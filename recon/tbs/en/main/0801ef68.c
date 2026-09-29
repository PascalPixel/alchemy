/* 2026-09-29: eight minutes of permutation: 2135 -> 860 (28 register-only,
 * 6 operand, 5 inserted, 1 deleted), kept here in natural form (locals
 * reordered, the index walk as a while loop, the bottom-row cursor reset
 * before its first store, the window cell as gWindowWork). The candidate is
 * a local optimum, not the reference's shape: its last-row test goes
 * through a flag (s32 last = rows - 1 == row), which frees the registers
 * the column loop needs but costs the five inserted instructions that build
 * the flag, where the reference branches on cmp/bne. Testing the row
 * directly gives 1865. A second 8-minute run from 860 with another seed
 * found nothing lower. */
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

extern u8 *gWindowWork;
extern const s8 Data_080371c4[];

void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags)
{
    u8 *base = gWindowWork;
    s32 first = 1;
    u32 rows = window->height;
    s32 bias = 0;
    u32 max = window->width - 1;
    s32 i;


    if ((flags & 1) == 0)
        flags &= ~2;
    if (flags & 2) {
        bias = 5;
        first = 0;
    }
    i = first;
    while (Data_080371c4[i] >= 0) {
        u32 col = Data_080371c4[i] + bias;
        if (col < max) {
            u32 row;
            for (row = 0; row != rows; row++) {
                u16 *dest = (u16 *)(2 * (((window->y + row) << 5) + (col + window->x)) + (u32)base);

                if (row == 0) {
                    *dest = 0xf018;
                } else {
                    s32 last = rows - 1 == row;

                    if (last)
                        *dest = 0xf019;
                    else
                        *dest = 0xf00f;
                }
            }
        }
        ++i;
    }
    if (base[0xea5]) {
        u32 col;
        u16 *dest = (u16 *)(((window->y + window->height) << 6) + ((u32)base + (window->x << 1)) - 64);
        col = 1;
        *dest++ = 0xf080;
        while (col < max) {
            col++;
            *dest++ = 0xf081;
        }
        *dest = 0xf082;
    }
    base[0xea3] = 1;
}
