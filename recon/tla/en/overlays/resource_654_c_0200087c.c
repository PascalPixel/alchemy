// Near miss, differ 2 of 77 (tla-en, 2026-10-30): the native issues the
// leader's z load one slot later after layer 2's store, filling the slot with
// the camera pointer's copy (mov r1, sl); alchemy-gcc issues the load at once.
// In sched2 both are ready and the load wins on priority (90 to 86): under
// -mthumb-slow-loads a load waits after a store only when no later store
// writes back to its address, and leader->z += 0x8000 is such a
// read-modify-write, so the store-to-load cost stays 1. Scheduler cost model,
// not source: alchemy permute (5 to 6 minutes on 26 October, 4 minutes on 30
// October) and an empty asm or one-pass block after any one statement do not
// move it; leader->z = leader->z + 0x8000 is the same; reading leader->z into
// a temporary before layer 2's update leaves 76 differing.
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"

void Func_0200087c(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *camera = Func_02003ad0();
  struct ShrineOfTheSeaGodObject *object8 = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *object9 = Actor_GetById(9);
  struct ShrineOfTheSeaGodObject *object10 = Actor_GetById(10);

  layer0->y += 0x8000;
  layer1->y += -0x8000;
  layer2->y += -0x8000;
  leader->z += 0x8000;
  camera->y += 0x8000;
  object8->z += 0x8000;
  object9->z += 0x8000;
  object10->z += 0x8000;
  wait(4);
}
