// First draft of Func_081053a8: loads the five menu icons of gData_08105a50
// (Func_08038288 mode 8) into free VRAM entries, sets up each slot's sprite
// attributes (a 0x40000000 shape word, then the entry's tile), keeps the five
// entries in the work and starts MenuIcon_PushSlots. Beside
// MenuIcon_PushSlots (games/tla/src/menu/menu_icon_slots.c) and
// MenuIcon_ReleaseSlots (menu_icon_release.c), whose work layout this uses.
// Differ 56 of 83 (draft 71). Left: the native keeps the table base in r11
// and its offset 2 * i in r9 with a separate counter in r8 counting down,
// saving r8 to r11; here i stays the counter, spilled to the stack around
// the calls, and only r8 is saved. Measured: indexing the slots and entries
// by i gives 82; a pointer walking the id table 54 (not the native's
// base-plus-offset load).
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

void Func_081053a8(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuSlotEntry *slot = menu->slots;
  u16 *resource = menu->resources;
  s32 entry;
  s32 i;

  for (i = 0; i < 5; i++) {
    entry = Resource_FindFreeEntry();
    if (entry <= 95) {
      u32 *attributes;

      Func_08038288(8, gData_08105a50[i], entry, 0);
      attributes = &slot->attributes;
      *attributes++ = 0x40000000;
      *attributes = 0;
      slot->tile = gResourceTableEntries[entry].offset >> 5;
    }
    *resource++ = entry;
    slot++;
  }
  menu->active = 1;
  Scheduler_AddOrUpdateCallback(MenuIcon_PushSlots, 0x48a);
}
