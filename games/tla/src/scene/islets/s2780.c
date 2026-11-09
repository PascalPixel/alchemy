#include "islets.h"
#include "scene_tables.h"

void Func_02002780(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;

  object = Actor_GetById(party->currentOwner);
  Func_02003e64();
  Event_ResolvePendingActions(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_SetModeById(party->currentOwner, 2);
  Actor_SetSpeedParameters(party->currentOwner, 0x8000, 0x4000);
  Actor_SetMoveTarget(object, ((8 - (object->x >> 16) % 16) << 16) + object->x,
                      object->y, object->z);
  Script_WaitForEventTimeout(object);
  object->angle = FACING_NORTH;
  Actor_SetMovementTypeById(party->currentOwner, gData_02004028);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}
