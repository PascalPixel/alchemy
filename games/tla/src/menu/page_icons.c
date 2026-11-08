#include "types.h"

struct MenuPageIcon {
  u8 reserved00[5];
  u8 state;
  u16 x;
  u16 y;
};

struct MenuPageWork {
  u8 reserved000[0x4c];
  struct MenuPageIcon *icons[32];
  u8 reserved0cc[0x148];
  u8 entryCount;
};

#include "ram_buffer.h"

void Icon_PrepareActor(struct MenuPageIcon *icon);

void Menu_SetPageIcons(s32 pageSize, s32 first, s32 window, s32 x, s32 y) {
  struct MenuPageWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuPageIcon *icon;
  s32 i;

  for (i = 0; i < 32; i++) {
    icon = menu->icons[i];
    if (icon != NULL)
      icon->state = 13;
  }
  for (i = first; i < pageSize + first && (icon = menu->icons[i]) != NULL &&
                  i <= menu->entryCount - 1;
       i++) {
    icon->x = x;
    icon->y = y + (i - first) * 16;
    Icon_PrepareActor(icon);
    icon->state = 1;
  }
}
