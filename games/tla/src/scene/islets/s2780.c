#include "islets.h"

void Func_02002780(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct Adventure *party = &gAdventure;
  struct ObjectRuntime *object;

  s32 cell;

  /* FAKEMATCH: the listing loads the adventure's address before it builds the
   * owner offset */
  asm volatile("" : "+l"(party));
  object = Object_GetById(party->currentOwner);
  Func_02003e64();
  Func_02003fa4(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Object_SetModeById(party->currentOwner, 2);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, party->currentOwner,
        0x8000, 0x4000);
  cell = object->x >> 16;
  Func_02003e0c(object, ((8 - cell % 16) << 16) + object->x, object->y,
                object->z);
  Func_02003e14(object);
  object->angle = 0xc000;
  ObjectMotion_EnableActionAndSetCallback(party->currentOwner, gData_02004028);
  WaitFrames(12);
  Func_02003ffc(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003f7c(work->exit);
}
