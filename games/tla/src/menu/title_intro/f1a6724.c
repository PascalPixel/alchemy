#include "types.h"
#include "edition.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "palette_buffers.h"

// This picture's tiles follow Func_081a6638's, 32 for each row it numbers.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define FIRST_TILE 96
#else
#define FIRST_TILE 64
#endif

void PaletteFade_SetCurrent(s32 index, s32 transform);
s32 Title_WaitOrPress(u32 frames);

s32 Func_081a6724(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600e800;
  tile = FIRST_TILE;
  for (row = 0; row < 20; row++) {
    if (row >= 7 && row <= 9) {
      for (i = 0; i < 32; i++) {
        *map++ = tile++ | 0x3000;
      }
    } else if (row == 10 || row == 11) {
      for (i = 0; i < 32; i++)
        *map++ = 0x3060;
    } else {
      for (i = 0; i < 32; i++)
        *map++ = 0x3040;
    }
  }
  map = (u16 *)0x0600e000;
  tile = FIRST_TILE + 96;
  for (row = 0; row < 20; row++) {
    if (row == 10 || row == 11) {
      for (i = 0; i < 32; i++) {
        *map++ = tile++ | 0x3000;
      }
    } else {
      for (i = 0; i < 32; i++)
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
