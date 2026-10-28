#include "types.h"
#include "ram_buffer.h"

void ItemMenu_PositionCategory(s32 category);
void WindowSprite_RedrawSavedRectFar(void *window);
void ItemMenu_DrawCategory(void *window, s32 category, s32 flags);

void Func_080fa84c(s32 category) {
  u8 *menu = Ram_HeapSlots->menuRuntime;

  ItemMenu_PositionCategory(category);
  WindowSprite_RedrawSavedRectFar(*(void **)(menu + 36));
  ItemMenu_DrawCategory(*(void **)(menu + 36), category, 0);
}
