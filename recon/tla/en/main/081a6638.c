// Measured 2026-11-05 (tla-en): differ 22 of 93.
// Draft, differ 22 of 93 (2026-11-05): shows a picture of the intro: fills BG3's map
// with the blank tile, numbers rows 9 and 10's tiles in order, sets BG3 and
// clears blending and the scroll, then fades in and out, returning 0 when a
// key cuts the wait short and 1 when it runs out. Left: other, in the tile
// counter: the native keeps it in r6 (saved) and steps it as (n << 16) +
// 0x10000 >> 16 (asrs), zero-extending a copy for the store; here it stays
// in r2 and steps as (n + 1) << 16 >> 16, so r6 is not saved and the row
// loop's registers shift (r4/r6). Tried: an s16 counter cast to u16 for the
// store (differ 64, loop.c strength-reduces its shifted form), the counter
// stepped inside the store (differ 88), 0x2000 | n or n + 0x2000, column <=
// 31 and (u32)(row - 9) <= 1 (no change). Closer, and written here: a u16 copy
// set from an s16 counter, which keeps loop.c from reducing it and gives r6
// and the (n << 16) + 0x10000 step. Left: the native zero-extends the copy
// after that step (lsls r2, lsrs r2), here it does not, and the scroll loop
// has the 0 in r2 and the count in r1, here the reverse.
#include "types.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "palette_buffers.h"

void PaletteFade_SetCurrent(s32 index, s32 transform);
s32 Title_WaitOrPress(u32 frames);

s32 Func_081a6638(void) {
  u16 *map;
  s16 tile;
  s32 row;
  s32 column;
  s32 i;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600e800;
  tile = 0;
  for (row = 0; row < 20; row++) {
    if (row == 9 || row == 10) {
      for (column = 0; column < 32; column++) {
        {
          u16 step = tile;

          tile = step + 1;
          *map++ = (s16)step | 0x2000;
        }
      }
    } else {
      for (column = 0; column < 32; column++)
        *map++ = 0x2000;
    }
  }
  REG_BG3CNT = 0x1d0a;
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;

  PaletteFade_SetCurrent(0, 0);
  PaletteFade_SetTarget(0x10000, 0);
  REG_DISPCNT = 0x800;
  PaletteFade_Start(40);
  if (Title_WaitOrPress(150))
    return 0;
  PaletteFade_SetTarget(2, 0);
  PaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  return 1;
}
