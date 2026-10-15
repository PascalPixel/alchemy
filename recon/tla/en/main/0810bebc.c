/* Near miss: differ about 60 of 148. The native keeps both loop counters in
 * r4, saved around each call (str r4, [sp]), walks the resource list by
 * pointer in both loops, and clears the sprite attributes with a
 * post-incremented pair (stmia r2!, {r3}; str r3, [r2]) of 0x40000000 and 0.
 * The draft keeps its counter in r8 and copies the template from memory.
 * Tried: indexed lists (the counter then lands in r4 but the pointers do not
 * walk), two word stores. */
#include "item_icon.h"

void Func_0810bebc(u16 *items, s32 count, s32 mode)
{
    struct IconWork *work = Ram_HeapSlots->menu_runtime;
    struct IconSprite *sprite;
    u16 *resource = work->resources;
    s32 i;

    for (i = ICON_COUNT - 1; i >= 0; i--) {
        if (*resource != ICON_NONE) {
            Resource_ResetEntry(*resource);
            *resource = ICON_NONE;
        }
        resource++;
    }
    sprite = work->sprites;
    resource = work->resources;
    for (i = 0; i < ICON_COUNT; i++) {
        if (i == count)
            break;
        if (*items != 0) {
            s32 entry = Resource_FindFreeEntry();

            if (entry < ICON_NONE) {
                static const struct IconAttr blank = { 0, 0, 0, 0x20 };

                UiIcon_CopyResourceToSlotFar(*items, mode, entry);
                sprite->attr = blank;
                sprite->attr.tile = ResourceTableEntries[entry].offset >> 5;
            }
            *resource = entry;
        }
        resource++;
        items++;
        sprite++;
    }
}
