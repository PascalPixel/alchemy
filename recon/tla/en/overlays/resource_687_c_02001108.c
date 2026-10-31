// Near miss, differ 269 of 350 (tla-en, 2026-10-31): the control flow, calls,
// frame (the fill word at sp+98 above the two effect options at sp+56 and
// sp+16) and data accesses now follow the listing; what is left is loop
// motion and allocation. In the fade, the native keeps gData_020026c0 in r11
// from the outer loop's body and rematerializes the shifted-field mask 31
// from the pool inside the inner loop; here the mask is hoisted out of both
// loops into r9 and the table address is reloaded inside instead. The native
// also keeps each colour component sign-extended (lsls/asrs) at its stores,
// where GCSE here moves the join's zero-extension into the branches and
// combine drops the asrs. The spark and shower loops are goto loops because
// the native moves nothing out of them; the shower's options address is
// still hoisted here by GCSE. The rest is reload's choice of low registers
// for the counter in r8. Destined for scene/sea_of_time.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "call.h"
#include "scene_ids.h"
#include "scene_tables.h"

/// The 14 colours of background palette 3 (entries 1 to 14) that the fade
/// returns to, copied from palette RAM by Func_02001cdc.
extern u16 gData_020026c0[];

void Func_020010a8(struct SeaOfTimeObject *object);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02001f74(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f4c(struct SeaOfTimeObject *object, s32 value);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_0200200c(s32 a, s32 b);
void wait(s32 frames);

void Func_02001108(void) {
  volatile u16 fill;
  struct SeaOfTimeSpark burst;
  struct SeaOfTimeSpark rain;
  struct SeaOfTimeObject *object;
  s32 changed;
  u32 i;

  Func_02001fac();
  Func_02002044(0);
  GameFlag_SetBit(0x8dd);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Actor_GetById(8)->facing = FACING_NORTH;
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 8, 352, 264);
  Func_02001fec(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(352 << 16, -1, 196 << 16, 1);
  Func_02001ffc();
  Actor_WaitForMoveById(8);
  Func_02002054(78);
  Func_02002054(556);
  // Fills palette 3's entries 1 to 14 with one colour.
  fill = RGB(11, 6, 23);
  Dma_Set((const void *)&fill, (void *)&BG_PLTT_COLOR(3, 1),
          (DMA_ENABLE | DMA_SRC_FIXED) << 16 | 14, REG_DMA3);
  Func_02001f74(49, 70, 19, 73, 6, 6);
  // Steps each colour's red, green and blue by one toward the saved palette
  // every 10 frames until none changes. As a goto loop the palette pointer
  // loses the strength reduction the native shows (differ 265).
  do {
    wait(10);
    changed = 0;
    for (i = 1; i != 15; i++) {
      u16 color = BG_PLTT_COLOR(3, i);
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
      BG_PLTT_COLOR(3, i) = r | (g << 5) | (b << 10);
    }
  } while (changed);
  FieldEvent_Wait(30);
  // For 360 frames, spawns an effect every 16 frames, then every 8, then
  // every other frame, and plays sound 164 every 64 frames. As a C loop,
  // loop.c moves the mask, the palette and the options address out of it,
  // which the native does not (differ 310).
  i = 0;
sparks:
  if ((i & 63) == 0)
    Func_02002054(164);
  if (i < 120) {
    if (i & 15)
      goto next;
  } else if (i < 240) {
    if (i & 7)
      goto next;
  } else if (i & 1)
    goto next;
  {
    s32 x = Random16Far();
    s32 z = Random16Far();

    burst.kind = 7;
    Func_0200015c(((x & 31) << 16) + 0x1500000, 0, ((z & 31) << 16) + 0xb00000,
                  0, 0, 0, 0x10000, &burst);
    wait(1);
  }
next:
  i++;
  if (i != 360)
    goto sparks;
  // Then 16 larger effects, one a frame. As a C loop, loop.c also moves the
  // mask and the options address out, which the native does not (differ 265).
  i = 0;
shower:
  {
    s32 x = Random16Far();
    s32 z = Random16Far();

    rain.scaleX = 0x18000;
    rain.scaleY = 0x18000;
    rain.kind = 7;
    rain.mode = 1;
    Func_0200015c(((x & 31) << 16) + 0x1500000, 0, ((z & 31) << 16) + 0xb00000,
                  0, 0x10000, 0, 0xb0000, &rain);
    wait(1);
  }
  i++;
  if (i != 16)
    goto shower;
  object = Import_ActorCreate(159, 0x15c0000, 0, 0xb80000);
  // The object's per-frame hook, struct Actor's update.
  *(void (**)(struct SeaOfTimeObject *))((u8 *)object + 0x6c) = Func_020010a8;
  FieldEvent_Wait(60);
  Func_02001f4c(object, 1);
  FieldEvent_Wait(40);
  gMainRoutineWork.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov687Room2, 4);
  Func_0200200c(12, 6);
  Func_02001fb4();
}
