#include "shrine_of_the_sea_god.h"

/// Copies a 6 by 56 rectangle of cells from (x, y) to (18, 6), then
/// moves the first two layers' y by -0x2000 a frame for 16 frames.
void Func_0200063c(s32 x, s32 y) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  u8 i;

  ShrineOfTheSeaGod_CopyRect(x, y, 6, 56, 18, 6);
  for (i = 0; i < 16; i++) {
    /* FAKEMATCH(temporary): the first layer's y read before the step is
     * loaded; as layer0->y += step, 2 of 33 instructions differ in tla-en. */
    s32 y0 = layer0->y;
    s32 step = -0x2000;
    s32 frames = 1;

    /* FAKEMATCH(barrier): the step loaded from the pool and WaitFrames' 1 set
     * in each pass, as the native does; as layer0->y += -0x2000 and
     * layer1->y += -0x2000 with WaitFrames(1), 10 of 33 instructions differ
     * in tla-en (the step hoisted and spilled). */
    asm volatile("" : "+r"(step), "+r"(frames));
    layer0->y = y0 + step;
    layer1->y += step;
    WaitFrames(frames);
  }
}
