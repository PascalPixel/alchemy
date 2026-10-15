#include "shrine_of_the_sea_god.h"

void Func_0200092c(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[0];

  layer->y += 0x10000;
  WaitFrames(4);
}
