#include "types.h"
#include "frame.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gData_02002590;
extern s16 gData_020025a0[2][160];

s32 Math_Sine(s32 angle);

void Func_02000b28(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  s16 *lines = gData_020025a0[gData_02002590 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    sine = Math_Sine((*count + (u8)gFrameCount + (layer->y >> 16)) << 9);
    v[0] = sine;
    *lines++ = (layer->x >> 16) + (sine >> 15) + *count % 3 - 1;
  }
  gData_02002590 ^= 1;
}
