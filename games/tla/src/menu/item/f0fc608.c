#include "types.h"
#include "ram_buffer.h"
#include "edition.h"

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
  u8 unk000[0x24];
  s32 itemWindow;
  u8 unk028[8];
  s32 infoWindow;
  u8 unk034[0x190];
  u16 items[32];
};

// The Japanese page draws its page icons and names further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_X 120
#define ENTRY_X 32
#else
#define PAGE_X 116
#define ENTRY_X 24
#endif

extern char MsgItemName[];

void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height,
                                 s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void RenderOutput_RedrawSavedRectFar(s32 window);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page,
                            s32 style);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x,
                       s32 y);

/// Draws the names on one five-row page of the item list: The Broken Seal's
/// ItemMenu_DrawNamePage over this game's menu offsets.
s32 Func_080fc608(s32 window, s32 unused, const struct MenuResult *state) {
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;
  u32 page;
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  const u16 *itemId;

  RenderOutput_RedrawSavedRectFar(window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

  page = state->page;
  firstEntry = page * 5;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > 5) {
    visibleCount = 5;
  }

  Menu_SetPageIcons(5, firstEntry, window, PAGE_X, 34);
  Menu_DrawPageIndicator(window, state->entryCount, 5, state->page, 15);

  row = 0;
  if (visibleCount > row) {
    itemId = &menu->items[firstEntry];
    do {
      UiText_DrawCharacterAtOffsetFar((itemId[0] & 0x1ff) + (s32)MsgItemName,
                                      menu->itemWindow, ENTRY_X, row * 16 + 8);
      row++;
      itemId++;
    } while (visibleCount > row);
  }

  return 1;
}
