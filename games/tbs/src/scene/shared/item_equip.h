/// @file
/// Colosseum Final 1, 2 and 3, and Goma Cave Entrance: giving a character an
/// item and putting it on.
#include "types.h"
#include "inventory.h"

/// Gives the character the item, then equip every slot of their 15 that holds
/// it.
UNUSED static void Scene_GiveAndEquipItem(s32 character, s32 item) {
  struct Unit *unit = Unit_Get(character);
  s32 i;

  Inventory_AddItem(character, item);
  for (i = 0; i <= 14; i++) {
    if (unit->inventory[i] == item) {
      Inventory_Equip(character, i);
    }
  }
}
