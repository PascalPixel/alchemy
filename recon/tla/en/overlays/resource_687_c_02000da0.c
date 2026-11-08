// Differ 2 of 150 (tla-en, 2026-11-08, -mtune=gba), from 12: the palette read and written as
// palette[i] from colour 0 (a giv loop.c reduces) in place of a stepped pointer. Left: the
// palette giv is reduced in loop.c's first pass and the table offset (sl) in its second,
// so the palette's mov r8 is scheduled before mov sl, r0; the native sets it last.
// Tried on this form, none better: i < 15 (6), one entry pointer per colour, a table
// pointer local (92 to 104), (gData_020026c0 - 1)[i] (99), the palette set before fade,
// do-while, *(palette + i), the target read first (64); permuter 6 minutes: no gain.
// Each of the 14 colours of BG palette 3 steps one unit per channel toward
// gData_020026c0, every 10 frames, until none changes: the fade written as
// Func_02001108 writes it. Earlier, with a stepped pointer (12): the for's init or
// step reordered (14 to 18), the palette set before the for (12 to 18), a
// do-while for the outer loop, declaration orders, s32 i, s16 or const table,
// a table pointer local (98), the table read before the colour (62), an
// unsigned `changed`; eight minutes of permute found nothing below 12.
#include "types.h"
#include "call.h"
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
  s32 changed;
  volatile u16 *palette;
  u32 i;

  Task_RemoveFar(Func_02000c50);
  Func_02001fac();
  Func_02002044(0);
  Func_02002054(0x22c);
  *gData_020022cc = 1;
fade:
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 0); i != 15; i++) {
      u16 color = palette[i];
      u16 target = gData_020026c0[i - 1];
      u16 r = color & 31;
      u16 g = (color >> 5) & 31;
      u16 b = (color >> 10) & 31;
      u16 tr = target & 31;
      u16 tg = (target >> 5) & 31;
      u16 tb = (target >> 10) & 31;

      if (r < tr) {
        r++;
        changed = 1;
      }
      if (r > tr) {
        r--;
        changed = 1;
      }
      if (g < tg) {
        g++;
        changed = 1;
      }
      if (g > tg) {
        g--;
        changed = 1;
      }
      if (b < tb) {
        b++;
        changed = 1;
      }
      if (b > tb) {
        b--;
        changed = 1;
      }
      palette[i] = RGB(r, g, b);
    }
    if (changed)
      goto fade;
  *gData_020022cc = 1;
  GameFlag_ClearBit(0x200);
  return Func_02001fb4();
}
