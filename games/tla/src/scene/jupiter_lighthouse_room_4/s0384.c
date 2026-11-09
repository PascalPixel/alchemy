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
  if (Event_GetFlag(2567))
    Func_0200017c();
  if (Event_GetFlag(2568))
    Func_020001ac();
  Func_02000300();
  flag = Event_GetFlag(265);
  if (flag == 0 && gMainRoutineWork.entrance == 11) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    leader->priorityFlags &= ~64;
    Func_02001e48();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetAnimationSpeed(leader, 64);
    leader->motionFlags = flag;
    leader->y = leader->terrainHeight = -0x780000;
    Event_StartScreenTransition();
    FieldEvent_Wait(10);
    for (i = 0; i <= 89 && leader->y < 0; i++) {
      leader->terrainHeight = leader->y = leader->y + 0x30000;
      leader->angle += 0x1800;
      System_Wait(2);
    }
    leader->terrainHeight = leader->y = 0;
    leader->angle = 0x4000;
    leader->motionFlags = 3;
    leader->velocityY = 0x30000;
    leader->gravity = 0x8000;
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    FieldEvent_Wait(40);
    leader->gravity = 0x10000;
    leader->priorityFlags |= 64;
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    Func_02001e50();
  }
  if (!Event_GetFlag(265) && gMainRoutineWork.entrance == 12)
    Func_02001f00(gMainRoutineWork.currentOwner);
  return 0;
}
