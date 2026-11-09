#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_0200267c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s16 *exit;
  s32 index;

  object = Actor_Get(party->currentOwner);
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
  FieldActor_SetMoveTarget(object, ((8 - (object->x >> 16) % 16) << 16) + object->x,
                      object->y, object->z);
  Actor_WaitForMove(object);
  object->angle = FACING_NORTH;
  exit = &work->exit;
  index = *exit;
  Event_PlaySound(158);
  index--;
  MapGrid_AnimateCells(gData_020046bc[index].value, gData_020046bc[index].pair.first, gData_020046bc[index].pair.second);
  Actor_SetScript(party->currentOwner, gData_02004028);
  System_Wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(*exit);
}
