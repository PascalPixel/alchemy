#include "types.h"
#include "ram_buffer.h"

struct MenuSlotEntry {
  u8 unk00[12];
};

struct MenuIconWork {
  u8 unk000[0xbcc];
  struct MenuSlotEntry slots[5];
  u8 unkc08[10];
  s8 active;
};

void Runtime_PushSlotEntry(struct MenuSlotEntry *slot, s32 value);

void MenuIcon_PushSlots(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuSlotEntry *slot;
  s32 i;

  /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
  __asm__ volatile("" : "+l"(menu));
  if (menu->active != 0) {
    slot = menu->slots;
    for (i = 4; i >= 0; i--) {
      Runtime_PushSlotEntry(slot, 240);
      slot++;
    }
  }
}
