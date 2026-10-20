// Draft, differ 2 of 27 (2026-10-19): Forge_FindRecipe, the forge's recipe
// for an item. The native copies the item to r1, starts 0x1ff in r3 and then
// loads the table (adds r1, r0, #0; movs r3, #128; ldr r0, =table; lsls r3, r3,
// #1). As plain C (key first) 4 of 27 differ; the empty volatile barrier after
// the table's load brings the load above the mask but also above movs r3, #128
// (differ 2). Non-volatile with an r3 clobber gives the same; three minutes of
// alchemy-devtools permute found nothing better.
#include "types.h"

struct ForgeRecipe {
  s16 item;
  s16 chance;
  s16 results[8];
  s16 weights[8];
};

extern struct ForgeRecipe gData_0810cc34[];

struct ForgeRecipe *Forge_FindRecipe(s32 item) {
  struct ForgeRecipe *recipe = gData_0810cc34;
  s32 key;

  asm volatile("" : "+l"(recipe));
  key = item & 0x1ff;
  for (; recipe->item != -1; recipe++) {
    if (recipe->item == key)
      return recipe;
  }
  return NULL;
}
