#include "types.h"
#include "ram_buffer.h"

// Psynergy menu workspace (known fields only).
struct PsynergyMenuState {
  u8 unk000[0x24];
  s32 listWindow;
  u8 unk028[8];
  s32 infoWindow;
  u8 unk034[0x190];
  u16 psynergies[32];
  u8 unk204[0x10];
  u8 psynergyCount;
  u8 unk215;
  u8 paneOwner[2];
  u8 unk218[0x4c];
  s8 selectedIndexByOwner[8];
};

struct MenuResult {
  s32 ownerState;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 entryCount;
  s32 selectedIndex;
};

s32 Unit_Get(s32 owner);

/// Split the selected slot owner's Psynergy into pages of five rows.
s32 Func_080fd80c(struct MenuResult *result, s32 index) {
  s32 ownerState;
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuRuntime;
  s32 entryCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selectedIndex;
  s32 owner;

  ownerState = Unit_Get(menu->paneOwner[index]);
  entryCount = menu->psynergyCount;
  owner = menu->paneOwner[index];
  selectedIndex = menu->selectedIndexByOwner[owner];
  if ((selectedIndex + 1) > entryCount) {
    selectedIndex = entryCount - 1;
  }
  page = selectedIndex / 5;
  row = selectedIndex % 5;
  pageCount = entryCount / 5;
  if (entryCount % 5 != 0) {
    pageCount++;
  }
  result->ownerState = ownerState;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = entryCount;
  result->selectedIndex = selectedIndex;
  return 1;
}
