#include "types.h"
#include "ram_buffer.h"

struct InventoryMenuDetailsState {
  u8 unk000[0x34];
  s32 detailsWindow;
  u8 unk038[0x134];
  u16 itemIds[0x100];
};

s32 Window_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void Palette_CopyObjectBankToBackground14(void);
void ItemMenu_DrawItemDetails(s32, s32);

s32 ItemMenu_OpenDetail(s32 itemIndex) {
  struct InventoryMenuDetailsState *menu = Ram_HeapSlots->menuRuntime;

  Window_UpdateOrCreate(&menu->detailsWindow, 0, 0, 13, 10, 2);
  Palette_CopyObjectBankToBackground14();

  if (menu->itemIds[itemIndex] != 0)
    ItemMenu_DrawItemDetails(menu->detailsWindow, menu->itemIds[itemIndex]);

  return 1;
}
