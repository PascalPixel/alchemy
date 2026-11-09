#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "event_work.h"
#include "event_inline.h"

extern s32 gData_0200629c;

void Func_02000cd8(s32 id, s32 offset, s32 limit);
void Func_02000e60(void);
void Func_02005158(void);

void Func_020032c8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *statue = Actor_GetById(10);
  struct Actor *ride = Func_080d4568();
  s16 i; u16 next;
  s32 limit;

  Func_020051c0();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  leader->motionFlags = 0;
  wait(1);
  GameFlag_ClearBit(0x208);
  GameFlag_SetBit(0x209);
  GameFlag_ClearBit(0x301);
  GameFlag_SetBit(0x300);
  Sound_Play(162);
  Func_02000e60();
  i = 0;
do {
  leader->y += 0x48000;
  statue->y += 0x48000;
  ride->y += 0x1cccc;
  limit = 0x1000;
  Func_02000cd8(12, 0x20000, limit);
  Func_02000cd8(17, 0x18000, limit);
  Func_02000cd8(18, 0x10000, limit);
  Func_02000cd8(19, 0x8000, limit);
  Func_02000cd8(20, 0, limit);
  next = i;
  next++;
  i = next;
} while (next <= 15);
  Func_02005158();
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
do {
  leader->y += -0x6c000;
  statue->y += -0x6c000;
  ride->y += -0x2b333;
  limit = 0x1800;
  Func_02000cf8(12, 0x30000, limit);
  Func_02000cf8(17, 0x24000, limit);
  Func_02000cf8(18, 0x18000, limit);
  Func_02000cf8(19, 0xc000, limit);
  Func_02000cf8(20, 0, limit);
  wait(1);
} while (Actor_GetById(20)->scaleY > limit);
  gData_0200629c = 0;
  GameFlag_ClearBit(0x20d);
  Sound_Play(390);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  Actor_SetModeById(10, 1);
  Actor_SetPositionAndResetMotion_(Actor_GetById(10), 0x1780000, 0, 0xf80000);
  statue->priorityFlags |= 2;
  Actor_SetSpritePriority(10, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_020051c8();
}
