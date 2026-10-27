// 26 October 2026, under -mthumb-slow-loads: differ 2 of 77: the native issues
// a load one slot later after a store, where alchemy-gcc issues it at once;
// alchemy permute (5 to 6 minutes) and an empty asm or one-pass block after any
// one statement do not move it.
// Near miss, differ 10: as Func_02000568, the layer pointers and the 0x8000
// constant are scheduled a few instructions away from the listing.
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"

void Func_0200087c(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Object_GetById(gPartyState.currentOwner);
  struct ShrineOfTheSeaGodObject *camera = Func_02003ad0();
  struct ShrineOfTheSeaGodObject *object8 = Object_GetById(8);
  struct ShrineOfTheSeaGodObject *object9 = Object_GetById(9);
  struct ShrineOfTheSeaGodObject *object10 = Object_GetById(10);

  layer0->y += 0x8000;
  layer1->y += -0x8000;
  layer2->y += -0x8000;
  leader->z += 0x8000;
  camera->y += 0x8000;
  object8->z += 0x8000;
  object9->z += 0x8000;
  object10->z += 0x8000;
  WaitTicks(4);
}
