#include "types.h"

struct ForgeRecipe {
  s16 item;
  s16 chance;
  s16 results[8];
  s16 weights[8];
};

extern struct ForgeRecipe gData_0810cc34[];

struct ForgeRecipe *Forge_FindRecipe(s32 item) {
  s32 key = item & 0x1ff;
  struct ForgeRecipe *recipe;

  for (recipe = gData_0810cc34; recipe->item != -1; recipe++) {
    if (recipe->item == key)
      return recipe;
  }
  return NULL;
}
