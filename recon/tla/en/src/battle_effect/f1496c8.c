#include "types.h"
#include "iwram_call.h"

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

s32 Trig_Sin(s32 angle);

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase, s32 bluePhase) {
  u16 palette[64];
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;
  palette[0] = 0;

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

  /* FAKEMATCH(wrapper): the copy through an inline forwarder, so the palette's
   * address is built anew for it; as a direct call, 47 of 96 instructions
   * differ in tla-en. */
  CopyWords_((void *)0x05000002, palette, sizeof(palette));
}
