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
    }
  }
  sprite = work->sprites;
  for (i = 0; i < ICON_COUNT && i != count; i++, sprite++) {
    if (items[i] != 0) {
      entry = Resource_FindFreeEntry();
      if (entry <= 95) {
        u32 *attr;

        Icon_CopyResourceToSlotFar(items[i], mode, entry);
        attr = (u32 *)&sprite->attr;
        *attr++ = 0x40000000;
        *attr = 0;
        sprite->attr.tile = gResourceTableEntries[entry].offset >> 5;
      }
      work->resources[i] = entry;
    }
  }
}
