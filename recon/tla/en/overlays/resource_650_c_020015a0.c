// Near miss, differ 10 of 53: the wave table builder. HBlank's
// Func_02001558 (wave.c) streams the line table this fills: each of the 160
// lines is the layer's y plus a sine of (frame - line - y + 160). Plain
// (layer unpinned, differ 28) global.c gives the layer r7 and the frame
// counter's address r4 saved around the call; the listing has the counter's
// address in r7 and the layer in r8, which happens only when the layer ranks
// below the address and caller-save is not profitable for it (4 * calls >=
// refs), while here it has 5 refs. Pinned to r8 (below) reload copies it into
// r3 for both reads where the listing uses r1 and then r2. Tried 2026-10-15:
// a s16 y field (reads the second y with ldrh), *line++ (line and i swap),
// line computed first (48), the map pointer as the local (46), the two
// statements after the loop swapped (39), an asm operand on the layer (44);
// permute 4 minutes found nothing better.
#include "types.h"
#include "ram_buffer.h"

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

s32 Math_Sine(s32 angle);

extern s32 gData_02003c10;
extern s16 gData_02003c14;
extern s16 gData_02003990[2][160];

void Func_020015a0(void) {
  register struct MapLayer *layer asm("r8") = &Ram_HeapSlots->mapWork->layer;
  s16 *line = gData_02003990[gData_02003c14];
  s32 i;

  for (i = 0; i != 160; i++)
    line[i] =
        (layer->y >> 16) +
        ((Math_Sine(((u8)gData_02003c10 - i - (layer->y >> 16) + 160) << 9) +
          0x10000) >>
         15);
  gData_02003c14 ^= 1;
  gData_02003c10++;
}
