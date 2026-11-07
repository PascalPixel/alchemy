#include "types.h"
#include "edition.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "palette_buffers.h"

// The picture numbers three rows in these editions, two in the others.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define NUMBERED_ROWS 3
#else
#define NUMBERED_ROWS 2
#endif

void PaletteFade_SetCurrent(s32 index, s32 transform);
s32 Title_WaitOrPress(u32 frames);

s32 Func_081a6638(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600e800;
  tile = 0;
  for (row = 0; row < 20; row++) {
    if (row >= 9 && row < 9 + NUMBERED_ROWS) {
      for (i = 0; i < 32; i++) {
        *map++ = tile++ | 0x2000;
      }
    } else {
      for (i = 0; i < 32; i++)
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
