#include "types.h"
#include "call.h"
#include "system/io_reg.h"

/// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to,
/// copied from palette RAM by Func_02001cdc and rotated by Func_02001650 in
/// step with BG palette entries 49 to 62.
struct SavedColors {
  u16 colors[14];
};

void Func_02000c50(void);
extern u32 gData_020022cc;
extern struct SavedColors gData_020026c0;
void Task_RemoveFar(void (*func)(void));
void Func_02001fac();
void Func_02002044();
void Func_02002054();
void wait();
void GameFlag_ClearBit();
s32 Func_02001fb4();

s32 Func_02000da0(void) {
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Task_RemoveFar(Func_02000c50);
  Func_02001fac();
  Func_02002044(0);
  Func_02002054(0x22c);
  gData_020022cc = 1;
  // Each of the 14 colours steps one unit per channel toward its saved colour
  // every 10 frames, until none changes.
fade:
  wait(10);
  changed = 0;
  palette = &BG_PLTT_COLOR(3, 0);
  // i counts palette entries from 1 and j saved colours from 0; the saved
  // colours are read through a pointer set after i, each pass.
  for (i = 1, saved = &gData_020026c0, j = 0; i != 15; i++, j++) {
    u16 color = palette[i];
    u16 target = saved->colors[j];
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
  gData_020022cc = 1;
  GameFlag_ClearBit(0x200);
  /* FAKEMATCH(signature): typed to return s32, passing on Func_02001fb4's,
   * though its target Event_End returns nothing; as void, 6 of 150 instructions
   * differ in tla-en. */
  return Func_02001fb4();
}
