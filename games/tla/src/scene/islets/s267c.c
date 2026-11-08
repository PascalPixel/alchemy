#include "islets.h"
#include "scene_tables.h"

void Func_0200267c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s16 *exit;
  s32 index;

  object = Actor_GetById(party->currentOwner);
  Func_02003e64();
  Func_02003fa4(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_SetModeById(party->currentOwner, 2);
  Actor_SetSpeedParameters(party->currentOwner, 0x8000, 0x4000);
  Func_02003e0c(object, ((8 - (object->x >> 16) % 16) << 16) + object->x,
                object->y, object->z);
  Func_02003e14(object);
  object->angle = FACING_NORTH;
  exit = &work->exit;
  index = *exit;
  Func_02003ffc(158);
  index--;
  Call3((void (*)())Func_02003e1c, gData_020046bc[index].value,
        gData_020046bc[index].pair.first, gData_020046bc[index].pair.second);
  Actor_SetMovementTypeById(party->currentOwner, gData_02004028);
  wait(12);
  Func_02003ffc(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003f7c(*exit);
}
