#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "vram_tab.h"
#include "callback_scheduler.h"

struct MenuSlotEntry {
  u8 unk00[4];
  u32 attributes;
  u16 tile : 10;
  u16 unk08 : 6;
  u8 unk0a[2];
};

struct MenuIconWork {
  u8 unk000[0xbcc];
  struct MenuSlotEntry slots[5];
  u16 resources[5];
  s8 active;
};

extern u16 gData_08105a50[5];
void Func_08038288(s32 mode, s32 id, s32 slot, s32 flags);
void MenuIcon_PushSlots(void);

/// Loads the five menu icons of gData_08105a50 into free VRAM entries, gives
/// each slot its sprite attributes and tile, keeps the entries for
/// MenuIcon_ReleaseSlots and starts MenuIcon_PushSlots.
void Func_081053a8(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuSlotEntry *slot = menu->slots;
  u16 *ids = gData_08105a50;
  u16 *resource = menu->resources;
  s32 entry;
  s32 i;
  s32 offset = 0;

  for (i = 0; i < 5; i++) {
    entry = Resource_FindFreeEntry();
    if (entry <= 95) {
      u32 *attributes;

      // As ids[i] the table and index combine into one walking pointer; the
      // byte offset kept apart is what matches.
      Func_08038288(8, *(u16 *)(offset + (u32)ids), entry, 0);
      attributes = &slot->attributes;
      *attributes++ = 0x40000000;
      *attributes = 0;
      slot->tile = gResourceTableEntries[entry].offset >> 5;
    }
    *resource++ = entry;
    slot++;
    offset += 2;
  }
  menu->active = 1;
  Scheduler_AddOrUpdateCallback(MenuIcon_PushSlots, 0x48a);
}
