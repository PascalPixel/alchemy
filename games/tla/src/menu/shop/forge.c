#include "types.h"
#include "party_state.h"

struct ForgeRecipe {
  s16 item;
  s16 chance;
  s16 results[8];
  s16 weights[8];
};

struct ForgeRecipe *Forge_FindRecipe(s32 item);

s32 Forge_GetMaterialChance(s32 item) {
  struct ForgeRecipe *recipe = Forge_FindRecipe(item);

  if (recipe != NULL)
    return recipe->chance + 1;
  return 0;
}

struct ForgeRecipe *Forge_FindRecipe(s32 item);
u32 Random16(void);

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

u8 *Unit_Get(s32 owner);
s32 Forge_GetMaterialChance(s32 item);

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

s32 Party_ListActiveOwnersFar(u16 *owners);
s32 Forge_OwnerHasMaterial(s32 owner);

s32 Forge_PartyHasMaterial(void) {
  u16 owners[16];
  s32 count;
  s32 i;

  count = Party_ListActiveOwnersFar(owners);
  for (i = 0; i < count; i++) {
    if (Forge_OwnerHasMaterial(owners[i]) != 0)
      return 1;
  }
  return 0;
}

void Forge_SetItem(s32 item) { gPartyState.forgeItem = item & 0x1ff; }

s32 Forge_GetItem(void) {
  u32 item = gPartyState.forgeItem;
  u32 mask;

  /* FAKEMATCH: ⚓️ loads the item before it builds the mask */
  __asm__ volatile("" : "+l"(item));
  mask = 0x1ff;
  mask &= item;
  return mask;
}
