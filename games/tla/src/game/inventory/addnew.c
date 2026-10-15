#include "types.h"
#include "inventory.h"

void Func_080ae6c8(s32 item, s32 owner) {
  if (PartyInventory_FindOwner(item) == -1)
    Inventory_AddItem(owner, item);
}
