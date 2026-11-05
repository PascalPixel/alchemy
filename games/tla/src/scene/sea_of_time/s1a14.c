/// @file
/// The Sea of Time's opening: places the camera and the first effects, showers sparks
/// for 360 frames and larger effects for 16, then fades BG palette 3 to RGB(11, 6, 23).
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "system/io_reg.h"

struct EventWork687 {
  u8 unk000[0x16a];
  u16 unk16a;
};

extern u8 Func_020010a8[];
extern u8 Func_02001650[];
extern s32 gFrameCount;
extern s32 gData_020022cc;
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
void Func_02001928(s32 a, s32 b, s32 c);
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
  volatile u16 *palette;
  s32 params[10];
  s32 burst[10];

  Func_02001fac();
  Func_02002044(0);
  Func_02002054(0);
  Func_02001f74(49, 70, 19, 73, 6, 6);
  /* FAKEMATCH(wrapper): forwarded, so that its constants are built in the native's order;
   * as a direct call, 2 of 307 instructions differ in tla-en. */
  actor = (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 159, 0x15c0000, 0, 0xb80000);
  actor->update = (void (*)(void))Func_020010a8;
  /* FAKEMATCH(wrapper): forwarded, so that its constants are not shared with the second
   * move's; as a direct call, 47 of 307 instructions differ in tla-en. */
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1600000, -1, 0xc40000, 0);
  Event_StartScreenTransition();
  wait(1);
  Task_Add(Func_02001650, 0xc80);
  Event_WaitTransitionFrames();
  FieldEvent_Wait(60);
  Func_02001928(21, 23, 9);
  Func_02001928(37, 17, 10);
  Func_02001928(47, 27, 12);
  Func_02001928(59, 19, 13);
  Func_02001f94();
  Camera_MoveWithinMapBounds(0x1600000, -1, 0xc40000, 1);
  Func_02001ffc();
  GameFlag_SetBit(0x200);
  ((struct EventWork687 *)Ram_HeapSlots->eventWork)->unk16a = 200;
  Func_02002054(0x212);
  FieldEvent_Wait(40);
  for (i = 0; i != 360; i++) {
    if ((i & 31) == 0)
      Func_02002054(164);
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
      wait(1);
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
    wait(1);
  }
  Func_02001f64(actor);
  Func_02002054(0x22d);
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
    gData_020022cc += gFrameCount & 1;
  } while (changed);
  Task_RemoveFar(Func_02001650);
  Func_0200204c();
  GameFlag_SetBit(0x940);
  Func_02001fb4();
}
