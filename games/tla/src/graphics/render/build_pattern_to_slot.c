#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "glyph.h"
#include "item.h"
#include "ram_buffer.h"


s32 Ui_BuildPatternToSlot(s32 no, s32 unused, s32 slot)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(0x44, sizeof(GlyphTransfer));
    Ui_PrepareTransferFromTableEntry(no);
    VramBlock_LoadCached(slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x44);
    return 1;
}

extern u8 *UiIcon_ItemIconPointers[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 layer);

// Decodes item code's icon (the first icon for code 0) into the glyph work.
void Ui_PrepareTransferForItem(s32 code)
{
    GlyphTransfer *state = Ram_HeapSlots->glyphWork;
    struct ItemDefinition *item = Item_Get(0x1ff & code);

    if (code != 0) {
        state->encoded = UiIcon_ItemIconPointers[item->icon];
    } else {
        u8 *encoded;
        u8 **table = UiIcon_ItemIconPointers;

        /* FAKEMATCH: a no-op self-assignment keeps the item live here, so the table takes r2 as native; without it the table takes r3 (differ 4) */
        item->icon += 0;
        /* FAKEMATCH: the table address loads before the offset 0x604 is built and the entry after it, as native; without the barrier both follow the offset (differ 4) */
        asm volatile("");
        encoded = table[0];
        state->encoded = encoded;
    }
    state->width = 2;
    state->height = 2;
    UiGlyph_DecodeWithHeapRoutines(state, 0);
}
