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
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 cell;
  s32 id;

  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  cell = object->x >> 16;
  Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                object->z - 0x40000);
  Script_WaitForEventTimeout(object);
  object->angle = 0xc000;
  Sound_Play(128);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  id = gMainRoutineWork.currentOwner;
  if (entrance == 6)
    Actor_SetModeById(id, 25);
  else
    Actor_SetModeById(id, 26);
  wait(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(entrance);
}
