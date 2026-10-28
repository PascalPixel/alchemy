#include "islets.h"

void Func_0200267c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct Adventure *party = &gAdventure;
  struct Actor *object;
  s16 *exit;
  s32 index;
  s32 cell;

  /* FAKEMATCH: the listing loads the adventure's address before it builds the
   * owner offset */
  asm volatile("" : "+l"(party));
  object = Actor_GetById(party->currentOwner);
  Func_02003e64();
  Func_02003fa4(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_SetModeById(party->currentOwner, 2);
  Call3((void (*)())Actor_SetSpeedParameters, party->currentOwner,
        0x8000, 0x4000);
  cell = object->x >> 16;
  Func_02003e0c(object, ((8 - cell % 16) << 16) + object->x, object->y,
                object->z);
  Func_02003e14(object);
  object->angle = 0xc000;
  exit = &work->exit;
  index = *exit;
  /* FAKEMATCH: the listing loads the exit before it builds the cue number */
  asm volatile("" : "+l"(index));
  Call1((void (*)())Func_02003ffc, 158);
  /* FAKEMATCH: the listing subtracts one after the call, not before it */
  asm volatile("" : "+l"(index));
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
