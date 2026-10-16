// Near miss, differ 10: the listing reloads -0x2000 inside the loop; this
// draft hoists it and spills it across WaitFrames (sub sp, #12).
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"

void Func_0200063c(s32 x, s32 y) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  u8 i;

  ShrineOfTheSeaGod_CopyRect(x, y, 6, 56, 18, 6);
  for (i = 0; i < 16; i++) {
    layer0->y += -0x2000;
    layer1->y += -0x2000;
    WaitFrames(1);
  }
}
