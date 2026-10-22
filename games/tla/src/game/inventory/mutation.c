#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "inventory.h"

s32 Inventory_Discard(s32 owner, s32 slot) {
  s32 item = ((struct OwnerInventoryState *)Unit_Get(owner))->inventory[slot];
  s32 removed_slot = Inventory_Remove(owner, slot);

  if (removed_slot != -1) {
    Event_ClearInvalidPackedValuesFar(Item_AdjustCounter(item, 1));
  }
  return removed_slot;
}
