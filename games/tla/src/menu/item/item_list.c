#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ItemListMenu {
  u8 unk000[0xf0];
  struct WindowSprite *messageWindow;
};

void WindowSprite_RedrawSavedRectFar(struct WindowSprite *window);
s32 Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

s32 ItemMenu_IsSpecial(s32 item_id) {
  s32 result;
  s32 first;

  if (item_id > 0xC4) {
    goto L0;
  }
  first = 0xC1;
  if (item_id < first) {
    goto L0;
  }
  result = 1;
  return result;
L0:
  result = 0;
  return result;
}

void ItemMenu_DrawMessage(s32 unused, s32 message) {
  struct ItemListMenu *menu;

  menu = Ram_HeapSlots->menuRuntime;
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Text_DrawCharacterAtOffsetFar(message, (s32)menu->messageWindow, 0, 0);
}
