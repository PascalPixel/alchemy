/// @file
/// Colosso Log, River and Wall, and Goma Cave's entrance: giving a character an
/// item and putting it on.
#include "types.h"
#include "inventory.h"

/// Give the character the item, then equip every slot of their 15 that holds
/// it.
static void Scene_GiveAndEquipItem(s32 character, s32 item) {
  struct BattleUnit *unit = UnitGet(character);
  s32 i;

  Inventory_AddItem(character, item);
  for (i = 0; i <= 14; i++) {
    if (unit->inventory[i] == item) {
      Inventory_Equip(character, i);
    }
  }
}
