#include "types.h"
#include "item.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "version.h"
#include "menu_result.h"

struct InventoryMenuState {
  u8 unk000[0x30];
  void *infoWindow;
  s32 equipWindow;
  u8 unk038[0x18c];
  u16 items[32];
};

// The rows one page of the item list shows: Japanese fits six, the other
// editions five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

#define EQUIP_SLOT_X 2
// The equipment slots' highlight is a tile wider in the European editions.
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_JAPANESE
#define EQUIP_SLOT_WIDTH 9
#else
#define EQUIP_SLOT_WIDTH 10
#endif

void WindowSprite_RedrawSavedRectFar(void *window);
void WindowSprite_ClearListFar(void *window);
void wait(s32 frames);
void Text_DrawCharacterAtOffsetFar(u8 *message, void *window, s32 x, s32 y);
void Text_DrawMessageFar(s32 message, void *window, s32 x, s32 y);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y,
                                 s32 palette);
void Render_SetTilemapFlagRect(s32 window, s32 x, s32 y, s32 width, s32 height,
                               s32 flag);

s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;
  s32 base;
  s32 row;
  u32 entry;
  u32 mask;

  menu = Ram_HeapSlots->menuRuntime;
  base = state->page * PAGE_ROWS;
  state->selectedIndex = base + state->row;
  // The Japanese release clears the name's window; from patch 1 it redraws the
  // window's saved rectangle.
#if TLA_PATCH_1
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#else
  WindowSprite_ClearListFar(menu->infoWindow);
#endif
  wait(1);
  if (menu->items[state->selectedIndex] != 0) {
    // Japanese draws the name as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
    Text_DrawMessageFar((menu->items[state->selectedIndex] & ITEM_ID_MASK) +
                               (s32)MsgItemPlainName,
                           menu->infoWindow, 0, 0);
#else
    Text_DrawCharacterAtOffsetFar(
        (menu->items[state->selectedIndex] & ITEM_ID_MASK) + MsgItemPlainName,
        menu->infoWindow, 0, 0);
#endif
  }

  for (row = 0; row < PAGE_ROWS; row++) {
    if (row == state->row) {
      entry = menu->items[state->selectedIndex];
      mask = ITEM_ID_MASK;
      mask &= entry;
      item = Item_Get(mask);
      if (item->element != 4) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                    0);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 14);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 14);
      }
    } else {
      entry = menu->items[base + row];
      mask = ITEM_ID_MASK;
      mask &= entry;
      item = Item_Get(mask);
      if (item->element != 4) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                    4);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 15);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 15);
      }
    }
  }

  for (row = 0; row <= 3; row++)
    Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, row * 2 + 1,
                              EQUIP_SLOT_WIDTH, 1, 15);

  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_Get(menu->items[state->selectedIndex] & ITEM_ID_MASK);
    switch (item->type) {
    case 1:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 1,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 4:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 3,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 3:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 5,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 2:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 7,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    }
  }
  wait(1);
  return 1;
}
