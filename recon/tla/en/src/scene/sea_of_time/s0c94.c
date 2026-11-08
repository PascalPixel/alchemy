/// @file
/// Fades BG palette 3's entries 1 to 13 toward RGB(11, 6, 23), one unit per channel
/// every 10 frames, then hands the rest of the event to Func_02000c50's task.
#include "iwram_call.h"
#include "system/io_reg.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

extern s32 gFrameCount;
extern s32 gData_020022cc;
extern s32 gData_020022c8;
void Func_02000c50(void);
void Func_02001fac(void);
void Func_02002044(s32 skip);
void GameFlag_SetBit(s32 flag);
void Func_02002054(s32 sound);
void wait(s32 frames);
s32 Task_Add(void (*func)(void), s32 priority);
s32 Func_02001fb4(void);

s32 Func_02000c94(void) {
  volatile u16 *palette;
  s32 changed;
  s32 i;
  s32 frame;

  Func_02001fac();
  Func_02002044(0);
  GameFlag_SetBit(0x200);
  Func_02002054(SPLIT_CONSTANT(0x22d));
  do {
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 14; i++, palette++) {
      u16 color = *palette;
      u16 r = color & 31;
      u16 g = (color >> 5) & 31;
      u16 b = (color >> 10) & 31;

      if (r < 11) {
        r++;
        changed = 1;
      }
      if (r > 11) {
        r--;
        changed = 1;
      }
      if (g < 6) {
        g++;
        changed = 1;
      }
      if (g > 6) {
        g--;
        changed = 1;
      }
      if (b < 23) {
        b++;
        changed = 1;
      }
      if (b > 23) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    frame = gFrameCount;
    gData_020022cc += frame & 1;
  } while (changed);
  gData_020022cc = 360;
  gData_020022c8 = frame;
  Task_Add(Func_02000c50, 0x480);
  return Func_02001fb4();
}
