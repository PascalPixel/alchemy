#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"

struct MapLayer {
  u8 unk00[4];
  s32 y;
  u8 unk08[0x10];
  s32 unk18;
  s32 unk1c;
};

struct MapWork {
  u8 unk000[0x108];
  struct MapLayer layer;
};


extern s32 gData_02003c10;
extern s16 gData_02003c14;
extern s16 gData_02003990[2][160];

void Func_020015a0(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layer;
  s16 *line = gData_02003990[gData_02003c14];
  s32 i;

  for (i = 0; i != 160; i++) {
    s32 wave = Math_Sine(((u8)gData_02003c10 - i - (layer->y >> 16) + 160) << 9) +
               0x10000;

    line[i] = (layer->y >> 16) + (wave >> 15);
  }
  gData_02003c14 ^= 1;
  gData_02003c10++;
}
