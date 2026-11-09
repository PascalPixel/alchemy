#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 exit;
};

void Func_020056b4(struct Actor *object);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Jumps down the hole.
void Func_02001630(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct Actor *leader;
  struct EventWork154 *work;

  if (x == 28 && z == 9 && !(gInput.held & 0x10))
    return;
  if (x == 29 && z == 10 && !(gInput.held & 0x40))
    return;
  work = Ram_HeapSlots->eventWork;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  Func_020056b4(leader);
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 472, 144);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  FieldActor_SetMoveTarget(leader, PIXELS(472), 0, PIXELS(156));
  Script_WaitForEventTimeout(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}
