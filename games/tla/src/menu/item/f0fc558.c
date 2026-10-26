#include "types.h"
#include "ram_buffer.h"

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

extern char MsgItemPlainName[];
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 Render_SetTilemapFlagRect(s32 window, s32 x, s32 y, s32 width, s32 height,
                              s32 flag);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void WaitFrames(s32 frames);

/// Draws the selected item-list row's name and highlights only that row; the
/// twin of The Broken Seal's ItemMenu_DrawItemDetailPage.
s32 Func_080fc558(s32 arg0, void *arg1, struct MenuResult *state) {
  struct InventoryMenuState *menu;
  s32 page;
  s32 combined;
  s32 row;
  s32 window;

  page = state->page;
  menu = Ram_HeapSlots->menuRuntime;
  window = menu->infoWindow;
  combined = page * 5;
  combined += state->row;
  state->selectedIndex = combined;

  RenderOutput_RedrawSavedRectFar(window);
  WaitFrames(1);

  combined = state->selectedIndex;
  if (menu->items[combined] != 0) {
    s32 masked = (menu->items[combined] & 0x1ff) + (s32)MsgItemPlainName;

    UiText_DrawCharacterAtOffsetFar(masked, menu->infoWindow, 0, 0);
  }

  row = 0;
  do {
    if (row == state->row) {
      Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1, 14);
    } else {
      Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1, 15);
    }
    row++;
  } while (row <= 4);

  WaitFrames(1);
  return 1;
}
