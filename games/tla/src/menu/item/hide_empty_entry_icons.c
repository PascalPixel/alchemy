#include "types.h"
#include "ram_buffer.h"

struct ItemEntryIcon {
  u8 unk00[5];
  u8 state;
  u8 unk06[8];
  u8 slot;
};

struct ItemEntryMenu {
  u8 unk000[0x4c];
  struct ItemEntryIcon *entryIcons[32];
};

void UiIcon_PrepareObject(struct ItemEntryIcon *icon);

void Menu_HideEmptyEntryIcons(const u16 *items) {
  struct ItemEntryMenu *menu = Ram_HeapSlots->menuRuntime;
  s32 slot;

  for (slot = 0; slot < 32; slot++) {
    if (items[slot] == 0) {
      UiIcon_PrepareObject(menu->entryIcons[slot]);
      menu->entryIcons[slot]->state = 13;
      /* FAKEMATCH(barrier): an empty asm keeps the counter's reload after the
       * state store; without it, 2 of 32 instructions differ in tla-en. */
      __asm__("");
    }
  }
}
