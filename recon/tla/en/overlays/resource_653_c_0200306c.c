// Draft, differ 73 of 246: the first statue carries the leader up and
// sinks; Func_020032c8 is its twin. Both loops are goto loops, as the native
// builds their constants in each pass. Left: register choice (the leader and
// the ride in r6 and r7 the other way round).
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "event_work.h"

extern s32 gData_02006298;

void Func_02000cd8(s32 id, s32 offset, s32 limit);
void Func_02000d64(void);
void Func_02005158(void);
void Func_020053d8(s32 cue);

void Func_0200306c(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *statue = Object_GetById(9);
  struct ObjectRuntime *ride = Func_020052f0();
  s16 i;
  s32 limit;

  Func_020051c0();
  Func_02005370(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_020052f0()->motionFlags = 0;
  leader->motionFlags = 0;
  WaitFrames(1);
  GameFlag_SetBit(0x208);
  GameFlag_ClearBit(0x209);
  GameFlag_SetBit(0x301);
  GameFlag_ClearBit(0x300);
  Func_020053d8(162);
  Func_02000d64();
  i = 0;
rise:
  leader->y += 0x48000;
  statue->y += 0x48000;
  ride->y += 0x1cccc;
  limit = 0x1000;
  Func_02000cd8(11, 0x20000, limit);
  Func_02000cd8(13, 0x18000, limit);
  Func_02000cd8(14, 0x10000, limit);
  Func_02000cd8(15, 0x8000, limit);
  Func_02000cd8(16, 0, limit);
  i++;
  if ((u16)i <= 15)
    goto rise;
  Func_02005158();
  WaitFrames(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(10);
fall:
  leader->y += -0x6c000;
  statue->y += -0x6c000;
  ride->y += -0x2b333;
  limit = 0x1800;
  Func_02000cf8(11, 0x30000, limit);
  Func_02000cf8(13, 0x24000, limit);
  Func_02000cf8(14, 0x18000, limit);
  Func_02000cf8(15, 0xc000, limit);
  Func_02000cf8(16, 0, limit);
  WaitFrames(1);
  if (Object_GetById(16)->scaleY > limit)
    goto fall;
  gData_02006298 = 0;
  GameFlag_ClearBit(0x20c);
  Func_020053d8(390);
  Func_02005230(11, 0, 0);
  Func_02005230(13, 0, 0);
  Func_02005230(14, 0, 0);
  Func_02005230(15, 0, 0);
  Func_02005230(16, 0, 0);
  Object_SetModeById(9, 1);
  Object_SetPositionAndResetMotion(Object_GetById(9), 0xd80000, 0, 0xd80000);
  statue->priorityFlags |= 2;
  ObjectMotion_SetActionVariant(9, 3);
  Object_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                     1);
  WaitFrames(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_020051c8();
}
