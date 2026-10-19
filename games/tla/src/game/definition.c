//--------------------------------------------------------------------------
// definition.c
// Items: access definitions and check equipment eligibility.
//--------------------------------------------------------------------------

#include "battle.h"
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

//--------------------------------------------------------------------------
// Return the item's definition.
// item: item ID; ignore inventory equipment and quantity bits.
//--------------------------------------------------------------------------
struct ItemDefinition *Item_GetDirect(s32 item) {
  return gItem_DefinitionTable + (item & ITEM_ID_MASK);
}
#include "owner_state.h"
#include "battle_unit.h"

//--------------------------------------------------------------------------
// Test whether a character can equip the item.
// Check equipMask bit classId; return 0 for classId >= 8.
//--------------------------------------------------------------------------
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item) {
  struct OwnerInventoryState *st = UnitGet(unit);
  struct ItemDefinition *def = Item_GetDirect(item);
  s32 mask = def->equipMask;

  if (st->classId >= 8) {
    return 0;
  }
  return (mask >> st->classId) & 1;
}

//--------------------------------------------------------------------------
// Return 1 for non-equipment, or the character's equipment eligibility
// otherwise.
//--------------------------------------------------------------------------
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item) {
  if (Item_GetEquipmentGroup(item) == 0) {
    return 1;
  }
  return Item_CanOwnerEquipDirect(unit, item);
}
