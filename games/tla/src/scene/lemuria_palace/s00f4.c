#include "types.h"
#include "frame.h"
#include "io_reg.h"

extern s16 gData_02002608[];

static inline u32 Frame_Mod6(u32 frame) {
  /* FAKEMATCH(wrapper): the inline keeps gFrameCount's address out of a
   * callee-saved register; as gFrameCount % 6, 16 of 35 instructions differ in
   * tla-en. */
  return frame % 6;
}

/// Every sixth frame rotates colours 1-14 of background palette 12 by one
/// place; every frame sets its colour 15 from a twelve-step cycle.
void Func_020000f4(void) {
  /* FAKEMATCH(pin): the colour stays in r1; as a plain local, 4 of 35
   * instructions differ in tla-en. */
  register s32 color asm("r1");

  if (Frame_Mod6(gFrameCount) == 0) {
    volatile u16 *palette;
    /* FAKEMATCH(pin): the saved colour takes r1 and leaves r0 to the index; as
     * a plain local, 12 of 35 instructions differ in tla-en. */
    register s32 saved asm("r1");
    s32 index;

    {
      /* FAKEMATCH(pin): the read stays in r3; as a plain local, 6 of 35
       * instructions differ in tla-en. */
      register s32 raw asm("r3") = *(volatile u16 *)&BG_PLTT_COLOR(12, 14);

      /* FAKEMATCH(barrier): the barrier keeps the sign extension apart from
       * the read; without it, 6 of 35 instructions differ in tla-en. */
      asm("" : "+r"(raw));
      saved = (s16)raw;
    }
    index = 14;
  loop:
    palette = (volatile u16 *)&BG_PLTT_COLOR(12, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto loop;
    *(volatile u16 *)&BG_PLTT_COLOR(12, 1) = saved;
  }
  color = gData_02002608[(gFrameCount >> 2) % 12];
  *(volatile u16 *)&BG_PLTT_COLOR(12, 15) = color;
}
