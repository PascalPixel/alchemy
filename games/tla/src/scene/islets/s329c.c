#include "version.h"
#include "event_inline.h"

#if TLA_PATCH_1

#include "types.h"
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
void Func_02003410(void);
void PaletteQueue_Add(s32 a, s32 b, s32 c, s32 d);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02001c4c(void);
void Func_02002830(void);
void Func_020029c4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020034d8(void);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
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
  if (Event_GetFlag(0xaa5) && (done = Event_GetFlag(265)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(265)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(265)) {
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
void Func_02003410(void);
s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02001c4c(void);
void Func_02002830(void);
void Func_020029c4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020034d8(void);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
  Func_02003410();
  PaletteQueue_Add(3, 12, 5, 4);
  PaletteQueue_Add(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) && (done = Event_GetFlag(265)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(265)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(265)) {
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
