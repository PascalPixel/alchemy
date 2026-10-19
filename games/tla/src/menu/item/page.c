#include "types.h"
#include "ram_buffer.h"

// Item menu workspace (known fields only).
struct InventoryMenuState {
  u8 unk000[0x24];
  s32 itemWindow;
  u8 unk028[8];
  s32 infoWindow;
  u8 unk034[0x190];
  u16 items[32];
  u8 unk204[0x12];
  u8 paneOwner[2];
  u8 unk218[0x4c];
  s8 selectedIndexByOwner[8];
  u8 unk26c[0x282];
  s16 resourceSlots[2];
};

extern char ResourceId_MenuArrowTiles;
extern char ResourceId_MenuSmallDownArrowTiles;

s32 Resource_FindFreeEntry(void);
void VramBlock_LoadResourceFar(s32 slot, s32 size, s32 resource);

// Load two page arrows into free tile slots.
void Resource_LoadPairedBlocks(void) {
  struct InventoryMenuState *state = Ram_HeapSlots->menuRuntime;
  s32 value = Resource_FindFreeEntry();

  state->resourceSlots[0] = value;
  VramBlock_LoadResourceFar(value, 128, (s32)&ResourceId_MenuArrowTiles);
  value = Resource_FindFreeEntry();
  state->resourceSlots[1] = value;
  VramBlock_LoadResourceFar(value, 128,
                            (s32)&ResourceId_MenuSmallDownArrowTiles);
}

struct MenuResult {
  s32 ownerState;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 entryCount;
  s32 selectedIndex;
};

s32 UnitGet(s32 owner);
s32 ItemMenu_CountItems(s32 owner);

// Split the selected slot owner's items into pages of five rows.
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index) {
  s32 encoded;
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;
  s32 limit;
  s32 remainder;
  s32 quotient;
  s32 groups;
  s32 value;

  limit = ItemMenu_CountItems(menu->paneOwner[index]);
  encoded = UnitGet(menu->paneOwner[index]);
  value = menu->selectedIndexByOwner[menu->paneOwner[index]];
  if ((value + 1) > limit) {
    value = limit - 1;
  }
  quotient = value / 5;
  remainder = value % 5;
  groups = limit / 5;
  if (limit % 5 != 0) {
    groups++;
  }
  result->ownerState = encoded;
  result->page = quotient;
  result->pageCount = groups;
  result->row = remainder;
  result->entryCount = limit;
  result->selectedIndex = value;
  return 1;
}
