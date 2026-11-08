#include "shrine_of_the_sea_god.h"

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
