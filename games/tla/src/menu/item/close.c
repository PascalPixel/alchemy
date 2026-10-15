#include "types.h"
#include "ram_buffer.h"

void Menu_ReleaseEntryObjects(void);
void ItemMenu_HideAllIcons(void);
void WaitFrames(s32 frames);
void UiWindow_CloseIfOpen(void *window, s32 mode);

void ItemMenu_Close(void) {
  u8 *menu;
  u8 *cursor;

  menu = Ram_HeapSlots->menuRuntime;
  Menu_ReleaseEntryObjects();
  ItemMenu_HideAllIcons();
  WaitFrames(1);
  cursor = *(u8 **)(menu + 0x170);
  cursor[5] = 13;
  UiWindow_CloseIfOpen(menu + 0x10, 1);
  UiWindow_CloseIfOpen(menu + 0x24, 1);
  UiWindow_CloseIfOpen(menu + 0xf0, 1);
  UiWindow_CloseIfOpen(menu + 0x28, 1);
  UiWindow_CloseIfOpen(menu + 0x2c, 1);
  UiWindow_CloseIfOpen(menu + 0x30, 1);
  UiWindow_CloseIfOpen(menu + 0x34, 1);
  UiWindow_CloseIfOpen(menu + 0x38, 1);
  UiWindow_CloseIfOpen(menu + 0x3c, 1);
  UiWindow_CloseIfOpen(menu + 0x40, 1);
  UiWindow_CloseIfOpen(menu + 0x44, 1);
}

struct ItemMenuWork {
  u8 unk000[0x14];
  void *paneIcons[2];
  s8 paneIndex[2];
  u8 paneCount[2];
  u8 unk020[0x10];
  void *infoWindow;
  u8 unk034[0x190];
  u16 items[32];
  u16 ownerIds[8];
  u8 itemCount;
  u8 partyCount;
};

void RenderOutput_ClearListFar(void *window);
void Func_080f8ab4(s32 x, s32 y);
void *UnitGet(s32 unitId);
s32 ItemMenu_Collect(void *owner, u16 *items, s32 mode);
s32 Func_080fa5a4(u16 *ownerIds, u16 *items);
void UiIcon_PrepareObject(void *icon);

s32 ItemMenu_PrepOwner(s32 pane) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s32 result = 0;
  s32 index;
  void *owner;

  index = menu->paneIndex[pane];
  RenderOutput_ClearListFar(menu->infoWindow);
  menu->paneCount[pane] = menu->partyCount;
  if (index == -1) {
    menu->paneIndex[pane] = result;
    index = 0;
  } else {
    Func_080f8ab4(index * 24 - 10, 16);
  }
  owner = UnitGet(menu->ownerIds[index]);
  menu->itemCount = ItemMenu_Collect(owner, menu->items, 0);
  result = Func_080fa5a4(menu->ownerIds, menu->items);
  UiIcon_PrepareObject(menu->paneIcons[pane]);
  WaitFrames(1);
  return result;
}
