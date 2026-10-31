// Near miss, differ 2 of 78 (tla-en, 2026-10-31): after layer 2's store the
// native loads the -0x8000 constant for the leader and camera (ldr r2) before
// the leader's z (ldr r3, [r6, #16]); alchemy-gcc loads z first. In sched2
// both are ready at priority 90 and neither waits on the store (the z load
// opens a read-modify-write, so -mthumb-slow-loads keeps its cost at 1); the
// tie goes to the z load on its 8 dependents against the constant's 4.
// Scheduler cost model, not source, as in the twin Func_0200087c: alchemy
// permute (5 to 6 minutes on 26 October; 4 minutes, 10,283 candidates on 31
// October) and an empty asm or one-pass block after any one statement do not
// move it; one s32 local for the constant is the same; *z = *z - 0x8000
// through a pointer to leader->z leaves 21 differing.
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"

void Func_02000568(void) {
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

  layer0->y += -0x8000;
  layer1->y += 0x8000;
  layer2->y += 0x8000;
  leader->z += -0x8000;
  camera->y += -0x8000;
  object8->z += -0x8000;
  object9->z += -0x8000;
  object10->z += -0x8000;
  wait(4);
}
