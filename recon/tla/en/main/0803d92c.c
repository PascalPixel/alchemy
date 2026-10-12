/* Near miss, differ 4 of 43: The Broken Seal's Ui_PrepareTransferForItem. In
 * the arm for no item the native loads the icon table's address first and
 * its first entry between the halves of the field offset (ldr r2, =table;
 * movs r1, #192; lsls r1, r1, #3; ldr r2, [r2]; adds r1, #4); this form
 * starts the offset first and loads the entry after it. Measured: the entry
 * in a local gives the native order with the table in r3 for r2 (differ 4);
 * one pointer reused for table and entry, or the table behind a "+l" barrier
 * or a one-pass block, trade r2 and r3 through the store (differ 8); the
 * table pinned to r2 before a one-pass block lets the two arms share their
 * store (differ 23). */
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "glyph.h"
#include "item.h"
#include "ram_buffer.h"


void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);

void Ui_PrepareTransferForItem(s32 code)
{
    GlyphTransfer *state = Ram_HeapSlots->glyph_work;
    struct ItemDefinition *item = Item_Get(0x1ff & code);

    if (code != 0) {
        state->encoded = UiIcon_ItemIconPointers[item->icon];
    } else {
        state->encoded = UiIcon_ItemIconPointers[0];
    }
    state->width = 2;
    state->height = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}
