#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_020000c0(void);
void Func_0200017c(void);
void Func_020001ac(void);
void Func_02000300(void);
void Func_02001e48(void);
void Func_02001e50(void);
void Func_02001f00(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Event_StartScreenTransition(void);

s32 Func_02000384(void) {
  struct Actor *leader;
  s32 flag;
  s32 i;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  Func_020000c0();
  if (GameFlag_Test(2567))
    Func_0200017c();
  if (GameFlag_Test(2568))
    Func_020001ac();
  Func_02000300();
  flag = GameFlag_Test(265);
  if (flag == 0 && gMainRoutineWork.entrance == 11) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    leader->priorityFlags &= ~64;
    Func_02001e48();
    Event_ResolvePendingActions(0);
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
    Actor_SetAnimationSpeed(leader, 64);
    leader->motionFlags = flag;
    leader->y = leader->terrainHeight = -0x780000;
    Event_StartScreenTransition();
    Event_Wait(10);
    for (i = 0; i <= 89 && leader->y < 0; i++) {
      leader->terrainHeight = leader->y = leader->y + 0x30000;
      leader->angle += 0x1800;
      wait(2);
    }
    leader->terrainHeight = leader->y = 0;
    leader->angle = 0x4000;
    leader->motionFlags = 3;
    leader->velocityY = 0x30000;
    leader->gravity = 0x8000;
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    Event_Wait(40);
    leader->gravity = 0x10000;
    leader->priorityFlags |= 64;
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    Func_02001e50();
  }
  if (!GameFlag_Test(265) && gMainRoutineWork.entrance == 12)
    Func_02001f00(gMainRoutineWork.currentOwner);
  return 0;
}
