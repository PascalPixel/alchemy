// Measured under -Os, 5 November 2026: differ 81 (tla-en); notes below may predate it.
// Draft, differ 121 of 307, skeleton gap 35 (tla-en, 2026-11-01)
// Left: (1) the second spawn loop's `add r1, sp, #16` is hoisted out of the
// loop into r6, which shifts that loop's allocation and scheduling; the do and
// for forms hoist more, and the first loop escapes only through its branches.
// (2) The closing pack must keep `lsls #16; lsrs #6/#11/#16` while the
// sign-extensions in the steps survive: <<10 and <<5 are narrowed by the strh,
// and through a wide temporary or `<< 16 >> n` the lsl/asr pairs vanish.
// (3) Red's first compare needs a copy of the masked value (`adds r3, r4, #0`):
// with `color &= 31; red = color;` the copy returns (differ 92, gap 36) but the
// mask lands after green and blue.
// The steps' shape comes from s16 values read through u16; the loops are
// label and goto because a real loop hoists the constants the listing reloads.
// Measured 5 November 2026 (-Os): differ 22 of 307 (tla-en), plain loops and two
// parameter blocks; left, the colour read order and one orrs order.
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

extern u8 Func_020010a8[];
extern u8 Func_02001650[];
extern u8 gFrameCount[];
extern u8 gData_020022cc[];
void Func_02001fac();
void Func_02002044();
void Func_02002054();
void Func_02001f74();
s32 Import_ActorCreate();
void Camera_MoveWithinMapBounds();
void Event_StartScreenTransition();
void wait();
s32 Task_Add();
void Event_WaitTransitionFrames();
void FieldEvent_Wait();
s32 Func_02001928();
void Func_02001f94();
void Func_02001ffc();
void GameFlag_SetBit();
s32 Random16Far();
void Func_0200015c();
void Func_02001f64();
void Task_RemoveFar();
void Func_0200204c();
s32 Func_02001fb4();
void Func_02001a14(void) {
  struct Actor *actor;
  u32 i;
  s32 changed;
  u16 *palette;
  s32 params[10];
  s32 burst[10];

  Func_02001fac();
  Call1((void (*)())Func_02002044, 0);
  Call1((void (*)())Func_02002054, 0);
  Call6((void (*)())Func_02001f74, 49, 70, 19, 73, 6, 6);
  actor = (struct Actor *)Import_ActorCreate(159, 0x15c0000, 0, 0xb80000);
  FIELD_AT(actor, s32, 108) = (s32)Func_020010a8;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1600000, -1, 0xc40000, 0);
  Event_StartScreenTransition();
  Call1((void (*)())wait, 1);
  Call2((void (*)())Task_Add, (s32)Func_02001650, 0xc80);
  Event_WaitTransitionFrames();
  Call1((void (*)())FieldEvent_Wait, 60);
  Call3((void (*)())Func_02001928, 21, 23, 9);
  Call3((void (*)())Func_02001928, 37, 17, 10);
  Call3((void (*)())Func_02001928, 47, 27, 12);
  Call3((void (*)())Func_02001928, 59, 19, 13);
  Func_02001f94();
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1600000, -1, 0xc40000, 1);
  Func_02001ffc();
  Call1((void (*)())GameFlag_SetBit, 0x200);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 362) = 200;
  Call1((void (*)())Func_02002054, 0x212);
  Call1((void (*)())FieldEvent_Wait, 40);
  for (i = 0; i != 360; i++) {
    if ((i & 31) == 0)
      Call1((void (*)())Func_02002054, 164);
    if (i < 120) {
      if (i & 15)
        continue;
    } else if (i < 240) {
      if (i & 7)
        continue;
    } else if (i & 1)
      continue;
    {
      s32 mask = 31;
      s32 x = Random16Far();
      s32 y = Random16Far();

      params[2] = 0xcccc;
      params[3] = 0xcccc;
      params[0] = 1;
      params[1] = 7;
      x &= mask;
      x <<= 16;
      x += 0x1500000;
      Func_0200015c(x, 0, ((y & mask) << 16) + 0xb00000, 0, 0, 0, 0xb0000,
                    params);
      Call1((void (*)())wait, 1);
    }
  }
  for (i = 0; i != 16; i++) {
    s32 mask = 31;
    s32 x = Random16Far();
    s32 y = Random16Far();

    burst[2] = 0x18000;
    burst[3] = 0x18000;
    burst[1] = 7;
    burst[0] = 1;
    x &= mask;
    x <<= 16;
    x += 0x1500000;
    Func_0200015c(x, 0, ((y & mask) << 16) + 0xb00000, 0, 0x10000, 0, 0xb0000,
                  burst);
    Call1((void (*)())wait, 1);
  }
  Func_02001f64(actor);
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
    *(s32 *)gData_020022cc += *(s32 *)gFrameCount & 1;
  } while (changed);
  Call1((void (*)())Task_RemoveFar, (s32)Func_02001650);
  Func_0200204c();
  Call1((void (*)())GameFlag_SetBit, 0x940);
  Func_02001fb4();
}
