#include "types.h"
#include "bg_scroll.h"

extern u32 gFrameTick;

/// Fills the HBlank word table: 32 words of 0x01ff01ff, 240 rising steps,
/// 48 more of 0x01ff01ff and 192 zeros.
void DisplayScroll_BuildHblankWordTable(u32 *table) {
  s32 i;
  u32 value;
  u32 step;

  /* FAKEMATCH: a one-pass block keeps the pool load of the fill word ahead of
   * the step's split constant; plain, the split's two instructions take the
   * lead (differ 2), as the scheduler prefers the instruction with a dependent.
   */
  do {
    value = 0x01ff01ff;
  } while (0);
  step = 0x10000;
  for (i = 0; i < 32; i++)
    *table++ = value;
  for (i = 0; i < 240; i++) {
    *table++ = step;
    step += 0x20002;
  }
  for (i = 0; i < 48; i++)
    *table++ = value;
  for (i = 0; i < 192; i++)
    *table++ = 0;
}

void DisplayScroll_StepPositionEveryFourFrames(void) {
  if ((gFrameTick & 3) == 0) {
    u32 decrement = 0xffff;

    gBgScroll[2].x += decrement;
    gBgScroll[3].x += decrement;
  }
}
