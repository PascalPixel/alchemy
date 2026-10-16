// Draft of overlay 65f Func_02000390 and Func_020003d8, differ 37 of 34
// each: the native keeps the palette register's address inside the loop
// (0x05000030 + i * 2 rebuilt each pass) and walks the table with two
// counters (the blue offset from 4 and the red offset from 0, six apart,
// green as blue - 2); every form tried here (a [8][3] table, a flat table
// with i * 3, a second index stepped by 3) is strength-reduced to three
// pointers.
#include "../../../../games/tla/src/scene/garoh/garoh.h"
#include "palbuf.h"

extern const u16 gData_020054ee[];
extern const u16 gData_020054b8[];

#define BG_PLTT_COLOR(n) (*(volatile u16 *)(0x05000000 + (n) * 2))

// Build BG palette 1 colors 8-15 from the table.
void Func_02000390(void) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;
  u32 i;
  s32 j;

  for (i = 0, j = 0; i < 8; i++, j += 3) {
    BG_PLTT_COLOR(24 + i) = gData_020054ee[j + 2] << 10 |
                            gData_020054ee[j + 1] << 5 | gData_020054ee[j];
    buffers->palette[24 + i] = BG_PLTT_COLOR(24 + i);
  }
}

// Build BG palette 6 colors 1-9 from the table.
void Func_020003d8(void) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;
  u32 i;
  s32 j;

  for (i = 0, j = 0; i < 9; i++, j += 3) {
    BG_PLTT_COLOR(97 + i) = gData_020054b8[j + 2] << 10 |
                            gData_020054b8[j + 1] << 5 | gData_020054b8[j];
    buffers->palette[97 + i] = BG_PLTT_COLOR(97 + i);
  }
}
