// Draft, differ 14 of 150 (tla-en, 2026-11-05). Each of the 14 colours of BG palette 3
// // steps one unit per channel toward gData_020026c0, every 10 frames, until none changes:
// // the fade written as Func_02001108 writes it. Left: palette and table swap r4 and r3 on
// // their way to r8 and r11, and one add's place.
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
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 15; i++, palette++) {
      u16 color = *palette;
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
      *palette = RGB(r, g, b);
    }
    if (changed)
      goto fade;
  *gData_020022cc = 1;
  GameFlag_ClearBit(0x200);
  return Func_02001fb4();
}
