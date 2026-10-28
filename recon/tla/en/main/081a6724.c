// Draft, differ 131 of 160: shows a two-layer picture of the intro, as
// Func_081a6638 does: BG3's map numbers rows 7 to 9 from tile 64 over a
// blank of 0x3040 with rows 10 and 11 of 0x3060, BG0's map numbers rows 10
// and 11 from tile 160; it fades in, blends BG0 in over 16 steps and fades
// out, returning 0 when a key cuts a wait short and 1 otherwise. Left:
// other, the tile counter as in Func_081a6638: the native keeps it in r6
// and steps it as (n << 16) + 0x10000 >> 16, zero-extending a copy for the
// store; here it takes r2 and steps as (n + 1) << 16 >> 16, so every
// register of the map loops shifts by one (r0/r1/r4/r5 against r1/r0/r5/r4),
// and the pool order follows. The setup, the fades and the waits match.
// Tried: an s16 counter cast to u16 for the store (differ 140).
#include "types.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "palbuf.h"

void PaletteFade_SetCurrent(s32 index, s32 transform);
s32 Title_WaitOrPress(u32 frames);

s32 Func_081a6724(void) {
  u16 *map;
  u16 tile;
  s32 row;
  s32 column;
  s32 i;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600e800;
  tile = 64;
  for (row = 0; row < 20; row++) {
    if (row >= 7 && row <= 9) {
      for (column = 0; column < 32; column++) {
        *map++ = tile | 0x3000;
        tile++;
      }
    } else if (row == 10 || row == 11) {
      for (column = 0; column < 32; column++)
        *map++ = 0x3060;
    } else {
      for (column = 0; column < 32; column++)
        *map++ = 0x3040;
    }
  }
  map = (u16 *)0x0600e000;
  tile = 160;
  for (row = 0; row < 20; row++) {
    if (row == 10 || row == 11) {
      for (column = 0; column < 32; column++) {
        *map++ = tile | 0x3000;
        tile++;
      }
    } else {
      for (column = 0; column < 32; column++)
        *map++ = 0x3040;
    }
  }
  REG_BG3CNT = 0x1d0a;
  REG_BG0CNT = 0x1c09;
  REG_BLDCNT = 0x0841;
  REG_BLDALPHA = 0x1000;
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;

  PaletteFade_SetCurrent(0, 0);
  PaletteFade_SetTarget(0x10000, 0);
  REG_DISPCNT = 0x900;
  PaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  for (i = 1; i <= 16; i++) {
    REG_BLDALPHA = i | 0x1000;
    if (Title_WaitOrPress(3))
      return 0;
  }
  if (Title_WaitOrPress(150))
    return 0;
  PaletteFade_SetTarget(2, 0);
  PaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  return 1;
}
