// Near miss, differ 23 of 96 (2026-10-26; 47 before): The Broken Seal's
// BattleEffect_UpdatePhasePalette (games/tbs/src/battle_effect/parts/prism.c)
// with ⚓️'s levels: each channel's offset plus 16, times the index, over 64.
// The levels written inside the loop (loop.c hoists the + 16, as the native
// adds it in place after the calls) and the palette indexed by the counter
// (its pointer starts from sp + 2 in two steps, as natively) give the
// native's loop exactly. Left: the native forms the palette's address afresh
// for the palette[0] store and for the final copy (mov r2, sp; mov r1, sp),
// where alchemy-gcc's cse2 keeps the first in a callee-saved register for
// the second, so one more register is saved; palette[0] cleared after the
// offsets, as ☀️ does, measured 29, and the permuter (4 minutes) found only a
// temporary for the blue phase (19, not kept).
#include "types.h"
#include "iwram_call.h"

s32 Trig_Sin(s32 angle);

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase, s32 bluePhase) {
  u16 palette[64];
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  palette[0] = 0;
  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;

  index = 1;
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (redOffset + 16) * index / 64;
    green = (greenOffset + 16) * index / 64;
    blue = (blueOffset + 16) * index / 64;

    if (red < 0)
      red = 0;
    if (red > 31)
      red = 31;
    if (green < 0)
      green = 0;
    if (green > 31)
      green = 31;
    if (blue < 0)
      blue = 0;
    if (blue > 31)
      blue = 31;

    palette[index] = (blue << 10) | (green << 5) | red;
    index++;
  } while (index != 64);

  Iwram_CopyWords((void *)0x05000002, palette, sizeof(palette));
}
