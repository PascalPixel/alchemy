#include "types.h"
#include "ram_buffer.h"

void ItemMenu_PosCategory(s32 category);
void RenderOutput_RedrawSavedRectFar(void *window);
void ItemMenu_DrawCategory(void *window, s32 category, s32 flags);

void Func_080fa84c(s32 category) {
  u8 *menu = Ram_HeapSlots->menuRuntime;

  ItemMenu_PosCategory(category);
  RenderOutput_RedrawSavedRectFar(*(void **)(menu + 36));
  ItemMenu_DrawCategory(*(void **)(menu + 36), category, 0);
}
