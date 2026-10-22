#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "a9_motion.h"
#include "types.h"
#include "ram_buffer.h"
#include "item.h"

struct EntryIconMenu {
  u8 unk000[0x4c];
  struct RenderOutput *entryIcons[32];
};

/// Places each of the 32 entry icons on the grid.
// The Broken Seal's Menu_PlaceEntryObjectsInGrid.
void Func_081005e4(s32 originX, s32 originY, s32 phase) {
  s32 i;
  struct RenderOutput *obj;
  struct RenderOutput **table;

  i = 0;
  table = ((struct EntryIconMenu *)Ram_HeapSlots->menuRuntime)->entryIcons;
  do {
    obj = *table++;
    if (obj != NULL)
      Menu_PlaceEntryObjectInGrid(obj, i, originX, originY, phase);
    i += 1;
  } while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index,
                                 s32 originX, s32 originY, s32 phase) {
  s32 no;

  no = index;
  if (no > 0x1F) {
    no = 0;
  }
  obj->y = no / phase * 0x10 + originY;
  obj->x = no % phase * 0x10 + originX;
  UiIcon_PrepareObject(obj);
}

struct CategoryItemIcon {
  u8 reserved00[6];
  s16 x;
  s16 y;
};

struct CategoryItemIconState {
  u8 reserved00[76];
  struct CategoryItemIcon *icons[32];
};

void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct CategoryItemIconState *state;
  struct CategoryItemIcon *icon;
  s32 i;

  state = Ram_HeapSlots->menuRuntime;
  ItemMenu_PositionCategory();
  for (i = 0; i < 15; i++) {
    if (items[i] != 0 && (items[i] & 0x200) != 0) {
      icon = state->icons[i];
      if (icon != 0) {
        switch (Item_Get(items[i] & 0x1ff)->type) {
        case 1:
          icon->x = 216;
          icon->y = 32;
          break;
        case 2:
          icon->x = 216;
          icon->y = 80;
          break;
        case 3:
          icon->x = 216;
          icon->y = 64;
          break;
        case 4:
          icon->x = 216;
          icon->y = 48;
          break;
        }
        UiIcon_PrepareObject(icon);
      }
    }
  }
}

void ItemMenu_PositionCategory(void) {
  struct CategoryItemIconState *state = Ram_HeapSlots->menuRuntime;
  s32 x = 248;
  struct CategoryItemIcon **entry = state->icons;
  s32 y = 168;
  s32 remaining = 31;

  do {
    struct CategoryItemIcon *icon = *entry++;

    if (icon != 0) {
      icon->x = x;
      icon->y = y;
      UiIcon_PrepareObject(icon);
    }
    remaining--;
  } while (remaining >= 0);
}
