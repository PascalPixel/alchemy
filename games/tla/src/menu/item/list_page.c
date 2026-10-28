#include "types.h"
#include "ram_buffer.h"
#include "item.h"
#include "battle_unit.h"
#include "text/msg_ids.h"
#include "edition.h"

/// The rows of a page of the item list: six in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

// The page icons, the item names, the "no items" message and the Attack and
// Defense figures, as each translation lays them out.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define PAGE_ICONS_X 119
#define ITEM_NAME_X 128
#else
#define PAGE_ICONS_X 123
#define ITEM_NAME_X 136
#endif
#if LANGUAGE == LANGUAGE_GERMAN
#define EMPTY_MESSAGE_X 112
#else
#define EMPTY_MESSAGE_X 120
#endif
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define STAT_X 80
#else
#define STAT_X 72
#endif

struct MenuResult {
  s32 ownerState;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 entryCount;
  s32 selectedIndex;
};

struct InventoryMenuState {
  u8 unk000[0x1c4];
  u16 items[32];
  u8 unk204[0x10];
  u8 itemCount;
  u8 unk215;
  u8 paneOwner[2];
};

struct BattleUnit *Unit_Get(s32 owner);
void Window_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                    s32 height);
void Menu_SetPageIcons(s32 pageSize, s32 first, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 pageSize, s32 page,
                            s32 style);
void Text_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);

/// Draws one page of the member's items into window: five names a page, or
/// "no items", then the member's name, Attack and Defense.
s32 Func_081000e0(s32 window, s32 unused, struct MenuResult *state) {
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit;
  u16 *inventoryEntry;
  s32 firstEntry;
  u32 rowCount;
  u32 rowIndex;
  u8 *message;

  unit = Unit_Get(menu->paneOwner[0]);
  Window_ClearInteriorTilesFar(window, 128, 8, 224, 96);
  firstEntry = state->page * PAGE_ROWS;
  rowCount = (u8)(state->entryCount - firstEntry);
  if (rowCount > PAGE_ROWS)
    rowCount = PAGE_ROWS;
  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, PAGE_ICONS_X, 52);
  Menu_DrawPageIndicator(window, state->entryCount, PAGE_ROWS, state->page, 28);
  if (menu->itemCount == 0) {
    Text_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, window, EMPTY_MESSAGE_X,
                                    8);
  } else {
    rowIndex = 0;
    if (rowCount > rowIndex) {
      inventoryEntry = &menu->items[firstEntry];
      do {
        Text_DrawCharacterAtOffsetFar(
            (*inventoryEntry & ITEM_ID_MASK) + MsgItemName, window, ITEM_NAME_X,
            (rowIndex << 4) + 8);
        rowIndex = (u8)(rowIndex + 1);
        inventoryEntry++;
      } while (rowCount > rowIndex);
    }
  }
  Text_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  message = MsgStatLabel;
  Text_DrawCharacterAtOffsetFar(message, window, 32, 16);
  Text_DrawCharacterAtOffsetFar(message + 1, window, 32, 24);
  Text_DrawNumberInWindowFar(unit->atk, 3, window, STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->def, 3, window, STAT_X, 24);
  return 1;
}
