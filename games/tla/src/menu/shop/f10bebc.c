#include "item_icon.h"

/// Releases the item icons' VRAM entries, then loads the icons of up to count
/// items (an empty item keeps its old entry) and gives each its sprite
/// attributes and tile.
void Func_0810bebc(u16 *items, s32 count, s32 mode) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  struct IconSprite *sprite;
  s32 entry;
  s32 i;

  for (i = 0; i < ICON_COUNT; i++) {
    if (work->resources[i] != ICON_NONE) {
      Resource_ResetEntry(work->resources[i]);
      work->resources[i] = ICON_NONE;
      /* FAKEMATCH(barrier): keeps the store ahead of the reload of the
       * caller-saved counter; as plain statements, 4 of 98 instructions
       * differ in tla-en (with the second barrier). */
      __asm__ volatile("");
    }
  }
  sprite = work->sprites;
  for (i = 0; i < ICON_COUNT && i != count; i++, sprite++) {
    if (items[i] != 0) {
      entry = Resource_FindFreeEntry();
      if (entry <= 95) {
        u32 *attr;

        UiIcon_CopyResourceToSlotFar(items[i], mode, entry);
        attr = (u32 *)&sprite->attr;
        *attr++ = 0x40000000;
        *attr = 0;
        sprite->attr.tile = gResourceTableEntries[entry].offset >> 5;
        /* FAKEMATCH(barrier): keeps the tile store ahead of the reload of
         * the caller-saved counter; as plain statements, 4 of 98
         * instructions differ in tla-en (with the first barrier). */
        __asm__ volatile("");
      }
      work->resources[i] = entry;
    }
  }
}
