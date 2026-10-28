// Draft, differ 64 of 247: Func_0200306c's twin for the second statue.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "event_work.h"

extern s32 gData_0200629c;

void Func_02000cd8(s32 id, s32 offset, s32 limit);
void Func_02000e60(void);
void Func_02005158(void);
void Func_020053d8(s32 cue);

void Func_020032c8(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *statue = Object_GetById(10);
  struct ObjectRuntime *ride = Func_020052f0();
  s16 i;
  s32 limit;

  Func_020051c0();
  Func_02005370(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_020052f0()->motionFlags = 0;
  leader->motionFlags = 0;
  WaitFrames(1);
  GameFlag_ClearBit(0x208);
  GameFlag_SetBit(0x209);
  GameFlag_ClearBit(0x301);
  GameFlag_SetBit(0x300);
  Func_020053d8(162);
  Func_02000e60();
  i = 0;
rise:
  leader->y += 0x48000;
  statue->y += 0x48000;
  ride->y += 0x1cccc;
  limit = 0x1000;
  Func_02000cd8(12, 0x20000, limit);
  Func_02000cd8(17, 0x18000, limit);
  Func_02000cd8(18, 0x10000, limit);
  Func_02000cd8(19, 0x8000, limit);
  Func_02000cd8(20, 0, limit);
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
  Func_02000cf8(12, 0x30000, limit);
  Func_02000cf8(17, 0x24000, limit);
  Func_02000cf8(18, 0x18000, limit);
  Func_02000cf8(19, 0xc000, limit);
  Func_02000cf8(20, 0, limit);
  WaitFrames(1);
  if (Object_GetById(20)->scaleY > limit)
    goto fall;
  gData_0200629c = 0;
  GameFlag_ClearBit(0x20d);
  Func_020053d8(390);
  Func_02005230(12, 0, 0);
  Func_02005230(17, 0, 0);
  Func_02005230(18, 0, 0);
  Func_02005230(19, 0, 0);
  Func_02005230(20, 0, 0);
  Object_SetModeById(10, 1);
  Object_SetPositionAndResetMotion(Object_GetById(10), 0x1780000, 0, 0xf80000);
  statue->priorityFlags |= 2;
  ObjectMotion_SetActionVariant(10, 3);
  Object_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                     1);
  WaitFrames(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_020051c8();
}
