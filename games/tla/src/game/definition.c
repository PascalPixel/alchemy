/// @file
/// Items: access definitions and check equipment eligibility.

#include "battle.h"
#include "call.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "item.h"
#include "edition.h"

/// Returns the item's definition.
/// item: item ID; the inventory's equipment and quantity bits are ignored.
struct ItemDefinition *Item_GetDirect(s32 item) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 10 instructions
   * differ in tla-en. */
  return gItem_DefinitionTable + (item & SPLIT_CONSTANT(ITEM_ID_MASK));
}
#include "owner_state.h"
#include "battle_unit.h"

/// Returns whether a character can equip the item: bit classId of its
/// equipMask, or 0 for classId 8 and up.
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item) {
  struct OwnerInventoryState *st = Unit_Get(unit);
  struct ItemDefinition *def = Item_GetDirect(item);
  s32 mask = def->equipMask;

  if (st->classId >= 8) {
    return 0;
  }
  return (mask >> st->classId) & 1;
}

/// Returns 1 for an item that is not equipment, otherwise whether the
/// character can equip it.
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item) {
  if (Item_GetEquipmentGroup(item) == 0) {
    return 1;
  }
  return Item_CanOwnerEquipDirect(unit, item);
}
