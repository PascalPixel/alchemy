#include "version.h"

#if TLA_PATCH_1

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003e64(void);
void Func_02003e6c(void);
void Event_ResolvePendingActions(s32 skip);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_02003410(void);
void PaletteQueue_Add(s32 a, s32 b, s32 c, s32 d);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
struct Actor *Actor_GetById(u32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Task_Add(void (*func)(void), s32 priority);
void Func_02001c4c(void);
void wait(s32 frames);
void Func_02002830(void);
void Func_020029c4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020034d8(void);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(303);
  Func_02003410();
  {
    s32 a1 = 12;
    s32 a2 = 5;
    s32 a3 = 4;
    s32 a0 = 3;

    /* FAKEMATCH(barrier): the native loads the first argument last; as the
     * plain call, 2 of 145 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2), "+l"(a3));
    PaletteQueue_Add(a0, a1, a2, a3);
  }
  PaletteQueue_Add(4, 12, 5, 4);
  if (GameFlag_Test(0xaa5) && (done = GameFlag_Test(265)) == 0) {
    Actor_SetHorizontalPositionWithTerrain(10, 0x1180000, 0x1d40000);
    Actor_GetById(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 9, a1,
            0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Task_Add(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    GameFlag_SetBit(0x217);
    if (!GameFlag_Test(265)) {
      {
        s32 a3 = 0x4000;

        Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 9,
              0x980000, 0x3000000, a3);
      }
      Actor_SetHorizontalPositionWithTerrain(10, 0xb00000, 0x3090000);
      wait(1);
    }
  }
  if (!GameFlag_Test(265)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else if (entrance != 5) {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Func_02003e6c();
}

#else

// The Japanese release's older form.

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003e64(void);
void Func_02003e6c(void);
void Event_ResolvePendingActions(s32 skip);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_02003410(void);
s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
struct Actor *Actor_GetById(u32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Task_Add(void (*func)(void), s32 priority);
void Func_02001c4c(void);
void wait(s32 frames);
void Func_02002830(void);
void Func_020029c4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020034d8(void);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(303);
  Func_02003410();
  PaletteQueue_Add(3, 12, 5, 4);
  PaletteQueue_Add(4, 12, 5, 4);
  if (GameFlag_Test(0xaa5) && (done = GameFlag_Test(265)) == 0) {
    Actor_SetHorizontalPositionWithTerrain(10, 0x1180000, 0x1d40000);
    Actor_GetById(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 9, a1,
            0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Task_Add(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    GameFlag_SetBit(0x217);
    if (!GameFlag_Test(265)) {
      {
        s32 a3 = 0x4000;

        Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 9,
              0x980000, 0x3000000, a3);
      }
      Actor_SetHorizontalPositionWithTerrain(10, 0xb00000, 0x3090000);
      wait(1);
    }
  }
  if (!GameFlag_Test(265)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Func_02003e6c();
}

#endif
