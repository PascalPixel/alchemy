#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct InventoryMenuState {
  u8 unk000[0xf0];
  s32 messageWindow;
  u8 unk0f4[0x78];
  u16 selectedItems[2];
  u8 unk170[0xa6];
  u8 paneOwner[2];
  struct RenderOutput *selectedItemIcon;
};

extern char MsgItemName[];

void *Unit_Get(s32 owner);
void Func_08038288(s32 mode, s32 item, s32 slot, s32 flags);
void UiIcon_PrepareObject(struct RenderOutput *icon);
void UiText_DrawStringAtOffsetFar(void *string, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

// Draws the selected item's icon, owner and item name in the heading.
void Func_080fc12c(void) {
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;

  Func_08038288(2, menu->selectedItems[0], (u8)menu->selectedItemIcon->index,
                0);
  menu->selectedItemIcon->active = 1;
  menu->selectedItemIcon->x = 112;
  menu->selectedItemIcon->y = 8;
  UiIcon_PrepareObject(menu->selectedItemIcon);
  UiText_DrawStringAtOffsetFar(Unit_Get(menu->paneOwner[0]), menu->messageWindow,
                               16, 0);
  UiText_DrawCharacterAtOffsetFar((menu->selectedItems[0] & 0x1ff) +
                                      (s32)MsgItemName,
                                  menu->messageWindow, 16, 8);
}
