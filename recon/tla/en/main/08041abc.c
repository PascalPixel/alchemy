/* Near miss, differ 2 of 85 (one reordered pair), 2026-10-15: The Broken
 * Seal's UiWindow_ClearTileAttributesInRect on this game's work, whose
 * tilemap starts at 8. Remaining: the native loads the alternate flag
 * (ldrb r3, [r5, #2]) before it adds the column to the cursor
 * (adds r1, r1, r0); this adds first (equal priority, the add's insn comes
 * first). Read before the cursor is formed the flag loads in place but
 * takes r9 instead of r8 (differ 12); pinned to r8 as well, its copy to r8
 * comes before the cursor's last add instead of after it (differ 2).
 * Operand-tied empty asms after each statement and the device search of
 * barriers, one-pass blocks, swaps and pins found nothing lower.
 */
#include "types.h"
#include "ram_buffer.h"
#include "window.h"

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = Ram_HeapSlots->window_tiles;
    u16 *cursor = &((struct UiRenderWork *)base)->tilemap[y][x];
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    u32 row;

    for (row = 0; row < height; row++) {
        u32 column;

        for (column = 0; column < width; column++) {
            u32 tile = *cursor++ & 0x3ff;

            if ((tile - 0x80) <= 0x7f || (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                u32 index = ((tile & 0xff) ^ 0x80);

                ((struct UiRenderWork *)base)->tile_attributes[index] &= 0xfc;
            }
        }
        cursor += 32 - width;
    }
}
