/* Near miss, differ 2 of 81 (one reordered pair), 2026-10-15: The Broken
 * Seal's UiWindow_MarkVisibleTileAttributes on this game's work, whose
 * tilemap starts at 8. Remaining: the native loads the alternate flag
 * (ldrb r7, [r5, #2]) before it copies the work pointer for the tilemap
 * cursor (adds r4, r5, #0); this copies first. In sched2 both have
 * priority 2 and the copy wins on dependents. Tried: every declaration
 * order, the cursor or the flag assigned later, the flag as u8 or s32, an
 * empty asm or one-pass block between them (the width's constant moves),
 * operand-tied empty asms on base, cursor, alternate and width after each
 * statement (304 variants), five permuter minutes.
 */
#include "types.h"
#include "ram_buffer.h"
#include "window.h"

void Func_08041b68(void)
{
    u8 *base = Ram_HeapSlots->windowTiles;
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    u16 *cursor = ((struct UiRenderWork *)base)->tilemap[0];
    s32 width = 30;
    s32 row;
    s32 x;

    for (row = 20; row != 0; row--) {
        for (x = 0; x < width; x++) {
            u32 tile = *cursor++ & 0x3ff;

            if ((tile - 0x80) <= 0x7f || (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                u32 index = ((tile & 0xff) ^ 0x80);

                ((struct UiRenderWork *)base)->tileAttributes[index] |= 2;
            }
        }
    }

    for (row = 0; row < 256; row++, base++) {
        if (((struct UiRenderWork *)base)->tileAttributes[0] == 1)
            ((struct UiRenderWork *)base)->tileAttributes[0] = 0;
    }
}
