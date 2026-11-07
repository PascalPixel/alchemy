#include "types.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[0x24];
  s32 itemWindow;
  void *statusWindow;
  void *helpWindow;
  s32 infoWindow;
  u8 unk034[0xc0];
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  u8 unk0f8[0x30];
  s16 ownerY[4];
};

void *Func_080f88d0(struct InventoryMenuState *work);
void Menu_PlaceOwnerEntryObjects(void *window, s32 originX, s32 originY);
s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

/// Sets up the item menu's first screen, the twin of The Broken Seal's
/// ItemMenu_Initialize: the cursor window with the party's figures
/// (Func_080f88d0, Menu_PlaceOwnerEntryObjects), the info window along the
/// bottom and an 8 by 2 cursor grid. The parameters are not used.
void Func_080fa368(s32 x, s32 y, s32 mode, s32 columns) {
  struct InventoryMenuState *menu =
      (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  s32 ownerIndex;

  Menu_PlaceOwnerEntryObjects(Func_080f88d0(menu), 2, 2);
  for (ownerIndex = 3; ownerIndex >= 0; ownerIndex--)
    menu->ownerY[ownerIndex] = 26;

  {
    u8 zero = 0;
    s32 style;
    menu->helpWindow = NULL;
    menu->statusWindow = NULL;
    style = 2;
    menu->infoWindow = Window_CreateFar(0, 17, 30, 3, style);
    menu->itemWindow = 0;
    menu->selectedColumn = zero;
    menu->selectedRow = zero;
    menu->columnCount = 8;
    menu->rowCount = style;
  }
}
