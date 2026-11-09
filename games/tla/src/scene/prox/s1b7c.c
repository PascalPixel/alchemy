#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02001b7c(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  s32 entrance = work->returnEntrance;
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 cell;
  s32 id;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  cell = object->x >> 16;
  FieldActor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                object->z - 0x40000);
  Actor_WaitForMove(object);
  object->angle = 0xc000;
  Event_PlaySound(128);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  id = gMainRoutineWork.currentOwner;
  if (entrance == 6)
    Actor_SetAnimation(id, 25);
  else
    Actor_SetAnimation(id, 26);
  System_Wait(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
}
