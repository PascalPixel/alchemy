// Draft, differ 8 of 57: The Broken Seal's ItemMenu_DrawItemHead (the icon must
// be the shared struct RenderOutput for the native's reloads of the icon
// pointer after each store). Four rows are scheduled the other way round: the
// icon load before the item offset 0x16c is started (ldr r3, [r5]; movs r2,
// #182), the 112 and the 8 before the icon reloads (movs r3, #112; ldr r2,
// [r5]; strh), and the item load before 0x1ff is started (ldrh r3, [r2]; movs
// r0, #128). One-pass blocks at each of the three places measured 8 to 17.
// MsgItemName is not named in the catalogs yet.
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

void *UnitGet(s32 owner);
void Func_08038288(s32 mode, s32 item, s32 slot, s32 flags);
void UiIcon_PrepareObject(struct RenderOutput *icon);
void UiText_DrawStringAtOffsetFar(void *string, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

// Draws the selected item's icon, owner and item name in the heading.
void ItemMenu_DrawItemHead(void) {
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;

  Func_08038288(2, menu->selectedItems[0], (u8)menu->selectedItemIcon->index,
                0);
  menu->selectedItemIcon->active = 1;
  menu->selectedItemIcon->x = 112;
  menu->selectedItemIcon->y = 8;
  UiIcon_PrepareObject(menu->selectedItemIcon);
  UiText_DrawStringAtOffsetFar(UnitGet(menu->paneOwner[0]), menu->messageWindow,
                               16, 0);
  UiText_DrawCharacterAtOffsetFar((menu->selectedItems[0] & 0x1ff) +
                                      (s32)MsgItemName,
                                  menu->messageWindow, 16, 8);
}
