#include "types.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "palette_buffers.h"

struct TitleScrollStep {
  s32 frames;
  s32 speed[4];
  u32 mask[4];
};

void PaletteFade_SetCurrent(s32 index, s32 transform);
s32 Title_WaitOrPress(u32 frames);
s32 Title_RunScrollStep(struct TitleScrollStep *step);
void Fade_Wait(void);

#define TITLE_STEP(f, s0, s1, s2, s3, m0, m1, m2, m3)                          \
  (step.frames = (f), step.speed[0] = (s0), step.speed[1] = (s1),              \
   step.speed[2] = (s2), step.speed[3] = (s3), step.mask[0] = (m0),            \
   step.mask[1] = (m1), step.mask[2] = (m2), step.mask[3] = (m3),              \
   Title_RunScrollStep(&step))

s32 Func_081a6154(s32 mode) {
  struct TitleScrollStep step;
  u16 *map;
  u16 tile;
  s32 i;
  s32 j;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600f800;
  tile = 0;
  for (j = 0; j < 16; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++;
    }
  }
  tile = 0;
  for (; j < 32; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++;
    }
  }
  map = (u16 *)0x0600f000;
  tile = 0;
  for (j = 0; j < 16; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++ | 0x1000;
    }
  }
  tile = 0;
  for (; j < 32; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++ | 0x1000;
    }
  }
  REG_BG1CNT = 0x1f02;
  REG_BG2CNT = 0x1e05;
  REG_BLDCNT = 0x244;
  REG_BLDALPHA = 0xa0b;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  if (mode == 0) {
    PaletteFade_Initialize();
  }
  PaletteFade_SetCurrent(2, 0);
  PaletteFade_SetTarget(0x10000, 0);
  REG_DISPCNT = 0x600;
  if (mode == 0) {
    PaletteFade_Start(1);
  } else {
    PaletteFade_Start(10);
  }

  if (mode == 0) {
    if (!TITLE_STEP(30, 2, 0, -3, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 2, 0, -2, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 0, -2, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 0, -1, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 0, -1, 0, 1, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 0, 0, -1, 0, 0, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(20, -1, 0, 0, 0, 1, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(20, -1, 0, 1, 0, 3, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(20, -1, 0, 1, 0, 3, 0, 1, 0))
      return 0;
    PaletteFade_SetTarget(2, 0);
    PaletteFade_Start(60);
    if (!TITLE_STEP(60, -1, 0, 1, 0, 3, 0, 1, 0))
      return 0;
  } else if (mode == 1) {
    if (!TITLE_STEP(40, 1, 1, -1, -1, 1, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(20, 1, 1, -1, -1, 3, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 1, -1, -1, 7, 3, 1, 0))
      return 0;
    if (!TITLE_STEP(10, 0, 1, 0, -1, 0, 3, 0, 0))
      return 0;
    if (!TITLE_STEP(30, -1, 0, 0, -1, 15, 1, 0, 0))
      return 0;
    if (!TITLE_STEP(20, -1, -1, 0, -1, 15, 1, 0, 0))
      return 0;
    if (!TITLE_STEP(60, -1, -1, 0, -2, 15, 1, 0, 0))
      return 0;
    PaletteFade_SetTarget(2, 0);
    PaletteFade_Start(60);
    if (!TITLE_STEP(60, -1, -1, 0, -2, 15, 1, 0, 0))
      return 0;
  } else {
    if (!TITLE_STEP(60, 0, -1, -1, 1, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(16, 8, -8, 8, 8, 0, 0, 0, 0))
      return 0;
    j = 2;
    for (i = 11; i < 16; i++) {
      REG_BLDALPHA = (i << 8) | i;
      gBgScroll[1].y += j + 16;
      gBgScroll[2].y += j + 8;
      j++;
      if (Title_WaitOrPress(1))
        return 0;
      gBgScroll[1].y += j + 16;
      gBgScroll[2].y += j + 8;
      j++;
      if (Title_WaitOrPress(1))
        return 0;
    }
    PaletteFade_SetTarget(SPLIT_CONSTANT(0x7f00, 255), 0);
    PaletteFade_Start(1);
    if (Title_WaitOrPress(2)) {
      Fade_Wait();
      return 0;
    }
  }
  return 1;
}
