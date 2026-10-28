#include "types.h"
#include "adventure.h"

void Inventory_EquipFar(s32 owner, s32 slot);
s32 Inventory_AddItemFar(s32 owner, s32 item);
u8 *Unit_Get(s32 owner);
void Func_081082c4(s32 mode, s32 shop);

s32 Debug_ApplyPresetItemsAndFlags(void) {
  gAdventure.coins = 200000;
  gAdventure.counterC = 28;
  Inventory_EquipFar(1, Inventory_AddItemFar(1, 0x48d));
  Inventory_EquipFar(0, Inventory_AddItemFar(0, 0x40b));
  Inventory_AddItemFar(2, 0xe7);
  Unit_Get(3)[0x131] = 1;
  Unit_Get(5)[0x131] = 1;
  Unit_Get(2)[0x140] = 1;
  Func_081082c4(0, 30);
  return 0;
}
