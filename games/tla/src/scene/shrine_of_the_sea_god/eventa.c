#include "shrine_of_the_sea_god.h"

void Func_0200092c(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[0];

  layer->y += 0x10000;
  WaitFrames(4);
}

// Copies a rectangle of cells from (x, y), then raises map layers 0 and 1 by
// 0x2000 a frame for 16 frames.
void Func_0200094c(s32 x, s32 y) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  u8 i;
  s32 frames;

  ShrineOfTheSeaGod_CopyRect(x, y, 6, 56, 18, 6);
  i = 0;
loop:
  layer0->y += 0x2000;
  frames = 1;
  /* FAKEMATCH(barrier): WaitFrames' argument is set before layer 1 is read;
   * as WaitFrames(1), 2 of 34 instructions differ in tla-en. */
  asm volatile("" : "+l"(frames));
  layer1->y += 0x2000;
  WaitFrames(frames);
  i++;
  if (i <= 15)
    goto loop;
}
