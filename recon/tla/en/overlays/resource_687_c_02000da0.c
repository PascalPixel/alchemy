// Draft, under -Os with plain loops and no forced temporaries (tla-en, 5 November 2026; the figure is in
// the commit). Each of the 14 colours of BG palette 3 steps one unit per channel toward gData_020026c0, every
// 10 frames, until none changes. The red masks are int-sized (movs #31) and the others halfword (the pool's
// 0x1f), and each compare reads its channel through a u16 local, as the native's arms show.
// Left: allocation, set by the order of the channel locals; local-alloc gives the target's shift, the pool's 31
// and the colour r2, r3, r1 in the native, and no order found does so together with the mask in r4.
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

  Task_RemoveFar(Func_02000c50);
  Func_02001fac();
  Func_02002044(0);
  Func_02002054(0x22c);
  *gData_020022cc = 1;
  do {
    s32 i, offset;
    volatile u16 *palette;
    u8 *table;

    wait(10);
    table = (u8 *)gData_020026c0;
    palette = &BG_PLTT_COLOR(3, 1);
    changed = 0;
    i = 1;
    offset = 0;
    for (; i != 15; i++) {
      s32 color = *palette;
      s16 target = *(s16 *)(offset + table);
      s32 wide = (u16)target;
      s32 narrow = (u16)color;
      s16 green = ((u16)color >> 5) & 31;
      s32 targetRed = wide & 31;
      u16 targetGreen = ((u16)target >> 5) & 31;
      u16 targetBlue = ((u16)target >> 10) & 31;
      s32 red = color & 31;
      s16 blue = ((u16)color >> 10) & 31;
      {
        u16 x = red;
        if (x < (u16)targetRed) {
          red = x + 1;
          changed = 1;
        }
      }
      {
        u16 x = red;
        if (x > (u16)targetRed) {
          red = (s16)(x - 1);
          changed = 1;
        }
      }
      {
        u16 x = green;
        if (x < targetGreen) {
          green = x + 1;
          changed = 1;
        }
      }
      {
        u16 x = green;
        if (x > targetGreen) {
          green = (u16)(x - 1);
          changed = 1;
        }
      }
      {
        u16 x = blue;
        if (x < targetBlue) {
          blue = x + 1;
          changed = 1;
        }
      }
      {
        u16 x = blue;
        if (x > targetBlue) {
          blue = (u16)(x - 1);
          changed = 1;
        }
      }
      color = ((u16)blue << 10) | ((u16)green << 5) | (u16)red;
      *palette = color;
      palette++;
      offset += 2;
    }
  } while (changed);
  *gData_020022cc = 1;
  GameFlag_ClearBit(0x200);
  return Func_02001fb4();
}
