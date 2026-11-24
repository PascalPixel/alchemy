/// @file
/// The forge: its recipes, the materials the party holds and the roll for the
/// result.
// Part of shop.c.

#include "shop_internal.h"
#include "shop/shop.h"

/// The forge's recipe for an item, or NULL when it has none; the table ends at
/// an item of -1.
const struct ForgeRecipe *Forge_FindRecipe(s32 itemId) {
  const struct ForgeRecipe *recipe;

  itemId &= INVENTORY_ITEM_ID_MASK;
  for (recipe = gForge_Recipes; recipe->item != -1; recipe++) {
    if (recipe->item == itemId)
      return recipe;
  }
  return NULL;
}

/// Returns what the forge makes of an item, counted from MsgItemCondition: 0
/// for an item it has no recipe for, then enum ForgeCondition plus one.
s32 Forge_GetItemCondition(s32 itemId) {
  const struct ForgeRecipe *recipe = Forge_FindRecipe(itemId);

  if (recipe != NULL)
    return recipe->condition + 1;
  return 0;
}

/// Rolls what the forge makes of a material: one of its recipe's results, each
/// as likely as its weight; 0 for an item with no recipe.
s32 Forge_RollResult(s32 itemId) {
  const struct ForgeRecipe *recipe = Forge_FindRecipe(itemId);
  s32 total = 0;
  s32 result = 0;
  s32 i;
  s32 roll;

  if (recipe != NULL) {
    for (i = 0; i < 8 && recipe->results[i] != 0; i++)
      total += recipe->weights[i];
    roll = Random_Below(total);
    for (i = 0; i < 8 && recipe->results[i] != 0; i++) {
      roll -= recipe->weights[i];
      if (roll < 0)
        break;
    }
    if (i != 8)
      result = recipe->results[i];
  }
  return result;
}

/// Returns 1 when the party member carries anything the forge takes.
s32 Forge_UnitHasMaterial(s32 unitId) {
  struct Unit *unit;
  u16 *inventory;
  s32 slot;

  unit = Unit_Get(unitId);
  slot = 0;
  inventory = unit->inventory;
  for (; slot < INVENTORY_SLOTS; slot++) {
    if (Forge_GetItemCondition(*inventory++) != 0)
      return 1;
  }
  return 0;
}

/// Returns 1 when anyone in the party carries anything the forge takes.
s32 Forge_PartyHasMaterial(void) {
  u16 partyMemberIds[16];
  s32 count;
  s32 i;

  count = Party_ListMembersFar(partyMemberIds);
  for (i = 0; i < count; i++) {
    if (Forge_UnitHasMaterial(partyMemberIds[i]) != 0)
      return 1;
  }
  return 0;
}

/// Records the item the forge is making, for the smith to hand over later.
void Forge_SetItem(s32 itemId) {
  gMainRoutineState.forgeItem = itemId & INVENTORY_ITEM_ID_MASK;
}

/// Returns the item the forge is making.
s32 Forge_GetItem(void) {
  u32 itemId = gMainRoutineState.forgeItem;
  u32 mask;

  mask = INVENTORY_ITEM_ID_MASK;
  mask &= itemId;
  return mask;
}
