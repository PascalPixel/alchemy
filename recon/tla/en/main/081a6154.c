// Draft, differ 499 of 567: the title sequence's scrolling backdrop: fills
// the maps at 0x0600f800 and 0x0600f000 (the second with palette 1), sets
// BG1, BG2 and blending, clears the scrolls, fades in, then runs one of three
// scripted runs of Title_RunScrollStep steps chosen by mode (0, 1 or any
// other), the last ending with a blend ramp; returns 0 when a key cuts it
// short and 1 when it runs to the end. Same calls, stores and control flow;
// left: the four map loops' registers and order (the native keeps the row in
// r7, loads 0 for REG_DISPCNT and 0x1000 from the pool and starts each inner
// count at 31 before the jump), which cascades into the step stores'
// registers (r0/r4 against r2/r3).
#include "types.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "palbuf.h"

struct TitleScrollStep {
  s32 frames;
  s32 speed[4];
  u32 mask[4];
};

void PaletteFade_SetCurrent(s32 index, s32 transform);
s32 Title_WaitTicksOrPress(u32 frames);
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
  s16 tile;
  s32 row;
  s32 column;
  s32 i;
  s32 speed;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600f800;
  tile = 0;
  for (row = 0; row < 16; row++) {
    for (column = 0; column < 32; column++) {
      *map++ = tile;
      tile++;
    }
  }
  tile = 0;
  for (; row < 32; row++) {
    for (column = 0; column < 32; column++) {
      *map++ = tile;
      tile++;
    }
  }
  map = (u16 *)0x0600f000;
  tile = 0;
  for (row = 0; row < 16; row++) {
    for (column = 0; column < 32; column++) {
      *map++ = (u16)tile | 0x1000;
      tile++;
    }
  }
  tile = 0;
  for (; row < 32; row++) {
    for (column = 0; column < 32; column++) {
      *map++ = (u16)tile | 0x1000;
      tile++;
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
    speed = 2;
    for (i = 11; i < 16; i++) {
      REG_BLDALPHA = (i << 8) | i;
      gBgScroll[1].y += speed + 16;
      gBgScroll[2].y += speed + 8;
      speed++;
      if (Title_WaitTicksOrPress(1))
        return 0;
      gBgScroll[1].y += speed + 16;
      gBgScroll[2].y += speed + 8;
      speed++;
      if (Title_WaitTicksOrPress(1))
        return 0;
    }
    PaletteFade_SetTarget(0x7fff, 0);
    PaletteFade_Start(1);
    if (Title_WaitTicksOrPress(2)) {
      Fade_Wait();
      return 0;
    }
  }
  return 1;
}
