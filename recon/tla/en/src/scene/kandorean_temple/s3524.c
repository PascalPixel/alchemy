#include "../../../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02003524(void) {
  Func_020051c0();
  Func_02005370(0);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

void Func_02003564(void) {
  Func_02003524();
  Func_020052f8(3);
}

void Func_02003574(void) {
  Func_02003524();
  Func_020052f8(4);
}

void Func_02003584(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;
  s32 delta;
  struct MainRoutineWork *party;

  Func_020051c0();
  Func_02005370(0);
  object->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  object->angle = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 16);
  for (i = 0; i <= 7; i++) {
    Actor_SetPositionAndResetMotion(object, object->x, object->y,
                                    object->z + PIXELS(3));
    wait(1);
  }
  FieldEvent_Wait(20);
  party = &gMainRoutineWork;
  object->angle = FACING_SOUTH;
  // Once-only block: keeps the original's order of the code around it.
  do {
  } while (0);
  Func_020052d0(party->currentOwner, 258);
  Actor_SetModeById(party->currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetModeById(party->currentOwner, 28);
  FieldEvent_Wait(10);
  Actor_SetSpritePriority(party->currentOwner, 3);
  for (i = 0; i <= 39; i++) {
    {
      s32 scale = object->scaleX;

      /* FAKEMATCH: one variable set twice in the loop keeps both constants
       * loaded on every pass; as plain constants, 8 instructions differ in
       * tla-en. */
      delta = -0x400;
      object->scaleX = scale + delta;
    }
    object->scaleY += delta;
    delta = -0x20000;
    object->y += delta;
    wait(1);
    if (i == 10)
      Func_020053d8(204);
    if (i == 20) {
      ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
      Event_FinishScreenTransition();
    }
  }
  Func_020052f8(5);
}
