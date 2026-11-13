/// @file
/// The forge: its recipes, the materials the party holds and the roll for the
/// result.
// Part of shop.c.

#include "../shop_internal.h"

struct ForgeRecipe {
  s16 item;
  s16 chance;
  s16 results[8];
  s16 weights[8];
};

extern struct ForgeRecipe gData_0810cc34[];

/// The forge's recipe for an item, or NULL when it has none; the table ends at
/// an item of -1.
struct ForgeRecipe *Forge_FindRecipe(s32 item) {
  struct ForgeRecipe *recipe;

  item &= 0x1ff;
  for (recipe = gData_0810cc34; recipe->item != -1; recipe++) {
    if (recipe->item == item)
      return recipe;
  }
  return NULL;
}

s32 Forge_GetMaterialChance(s32 item) {
  struct ForgeRecipe *recipe = Forge_FindRecipe(item);

  if (recipe != NULL)
    return recipe->chance + 1;
  return 0;
}

s32 Forge_RollResult(s32 item) {
  struct ForgeRecipe *recipe = Forge_FindRecipe(item);
  s32 total = 0;
  s32 result = 0;
  s32 i;
  s32 roll;

  if (recipe != NULL) {
    for (i = 0; i < 8 && recipe->results[i] != 0; i++)
      total += recipe->weights[i];
    roll = (Random16() * total) >> 16;
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

s32 Forge_OwnerHasMaterial(s32 owner) {
  u16 *item;
  s32 i;

  item = (u16 *)Unit_Get(owner);
  i = 0;
  item += 108;
  do {
    if (Forge_GetMaterialChance(*item++) != 0)
      return 1;
    i++;
  } while (i <= 14);
  return 0;
}

s32 Forge_PartyHasMaterial(void) {
  u16 owners[16];
  s32 count;
  s32 i;

  count = Party_ListMembersFar(owners);
  for (i = 0; i < count; i++) {
    if (Forge_OwnerHasMaterial(owners[i]) != 0)
      return 1;
  }
  return 0;
}

void Forge_SetItem(s32 item) { gMainRoutineWork.forgeItem = item & 0x1ff; }

s32 Forge_GetItem(void) {
  u32 item = gMainRoutineWork.forgeItem;
  u32 mask;

  mask = 0x1ff;
  mask &= item;
  return mask;
}
