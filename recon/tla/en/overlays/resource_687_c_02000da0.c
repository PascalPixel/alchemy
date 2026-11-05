// Measured under -Os, 5 November 2026: differ 145 (tla-en); notes below may predate it.
// Draft, differ 168 of 150, skeleton gap 21/17 (tla-en, 2026-11-01)
// Each of the 14 colours of BG palette 3 steps one unit per channel toward
// gData_020026c0, every 10 frames, until none changes. Rewritten from the listing
// with no devices. The loops must be goto: a real for hoists the 31 and 65535 the
// listing rebuilds in the body. u16 target gives its ldrsh, s32 color its ldrh, and
// `(u16)x +/- 1` narrows to the listing's add of 65535.
// Left, by cause:
// 1. The main gap. PRE (07.gcse) merges the `v << 16` of each channel's compare and
// of the final OR, so the arm's asrs dies and the shifted value lives in a register.
// The listing recomputes that shift every time. `x = v; x = (u16)x;` before each
// compare stops PRE and reproduces arm and join exactly, because x is set around its
// own shift (see hash_scan_set in gcse.c), but that is a forced temporary, so it is
// not kept here: find the plain form that has this effect. Not found so far: a
// shared or scoped u16 x, or s32 channels.
// 2. red and targetRed take a movs #31 (r4) while the other four ANDs take the pooled
// 0x1f; the listing also keeps `(u16)target` as its own lsrs before `ands r2, r4`.
// Tried 65 spellings of the pair (mask local, `x = 31; x &= v`, s32 or u16),
// and none gained; 432 type mixes settled the rest.
#include "types.h"
#include "system/io_reg.h"

extern u8 Func_02000c50[];
extern s32 gData_020022cc[];
extern u16 gData_020026c0[];
void Task_RemoveFar();
void Func_02001fac();
void Func_02002044();
void Func_02002054();
void wait();
void GameFlag_ClearBit();
s32 Func_02001fb4();

s32 Func_02000da0(void) {
  s32 i, changed, offset;
  volatile u16 *palette;
  u8 *table;

  Task_RemoveFar(Func_02000c50);
  Func_02001fac();
  Func_02002044(0);
  Func_02002054(0x22c);
  *gData_020022cc = 1;
fade:
  wait(10);
  changed = 0;
  i = 1;
  table = (u8 *)gData_020026c0;
  offset = 0;
  palette = &BG_PLTT_COLOR(3, 1);
step: {
  s32 color = *palette;
  u16 target = *(u16 *)(offset + table);
  u16 red = color & 31;
  s16 green = ((u16)color >> 5) & 31;
  s16 blue = ((u16)color >> 10) & 31;
  u16 targetRed = target & 31;
  u16 targetGreen = (target >> 5) & 31;
  u16 targetBlue = (target >> 10) & 31;
  if ((u16)red < targetRed) {
    red = (u16)red + 1;
    changed = 1;
  }
  if ((u16)red > targetRed) {
    red = (u16)red - 1;
    changed = 1;
  }
  if ((u16)green < targetGreen) {
    green = (u16)green + 1;
    changed = 1;
  }
  if ((u16)green > targetGreen) {
    green = (u16)green - 1;
    changed = 1;
  }
  if ((u16)blue < targetBlue) {
    blue = (u16)blue + 1;
    changed = 1;
  }
  if ((u16)blue > targetBlue) {
    blue = (u16)blue - 1;
    changed = 1;
  }
  color = ((u16)blue << 10) | ((u16)green << 5) | (u16)red;
  *palette = color;
}
  i++;
  palette++;
  offset += 2;
  if (i != 15)
    goto step;
  if (changed)
    goto fade;
  *gData_020022cc = 1;
  GameFlag_ClearBit(0x200);
  return Func_02001fb4();
}
