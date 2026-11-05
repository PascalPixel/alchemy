// Measured under -Os, 5 November 2026: differ 83 (tla-en); notes below may predate it.
// Draft, differ 113 of 116: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gFrameCount[];
extern u8 gData_020022cc[];
extern u8 gData_020022c8[];
extern u8 Func_02000c50[];
void Func_02001fac();
void Func_02002044();
void GameFlag_SetBit();
void Func_02002054();
void wait();
s32 Task_Add();
s32 Func_02001fb4();

s32 Func_02000c94(void) {
  u16 *palette;
  s32 changed;
  s32 i;
  s32 frame;

  Func_02001fac();
  Call1((void (*)())Func_02002044, 0);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  Call1((void (*)())Func_02002054, 0x22d);
  do {
    Call1((void (*)())wait, 10);
    palette = (u16 *)0x5000062;
    changed = 0;
    for (i = 1; i != 14; i++) {
      u32 color = *palette;
      s16 green = ((u16)color >> 5) & 31;
      s16 blue = ((u16)color >> 10) & 31;
      s32 red = color & 31;
      u16 level;

      level = red;
      if (level < 11) {
        red = level + 1;
        changed = 1;
      }
      level = red;
      if (level > 11) {
        red = (s16)(level - 1);
        changed = 1;
      }
      level = green;
      if (level < 6) {
        green = level + 1;
        changed = 1;
      }
      level = green;
      if (level > 6) {
        green = level - 1;
        changed = 1;
      }
      level = blue;
      if (level < 23) {
        blue = level + 1;
        changed = 1;
      }
      level = blue;
      if (level > 23) {
        blue = level - 1;
        changed = 1;
      }
      {
        s32 value = ((u16)blue << 10) | ((u16)green << 5) | (u16)red;

        *palette = value;
      }
      palette++;
    }
    frame = *(s32 *)gFrameCount;
    *(s32 *)gData_020022cc += frame & 1;
  } while (changed);
  *(s32 *)gData_020022cc = 360;
  *(s32 *)gData_020022c8 = frame;
  Call2((void (*)())Task_Add, (s32)Func_02000c50, 0x480);
  return Func_02001fb4();
}
