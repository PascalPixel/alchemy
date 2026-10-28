#include "local.h"

void Func_02003524(void) {
  Func_020051c0();
  Func_02005370(0);
  Actor_GetById(gSequenceParentWork.currentOwner)->motionFlags = 0;
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
  struct Actor *object = Actor_GetById(gSequenceParentWork.currentOwner);
  u32 i;
  s32 delta;
  struct SequenceParentWork *party;

  Func_020051c0();
  Func_02005370(0);
  object->motionFlags = 0;
  Actor_SetActionVariantValue(gSequenceParentWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  object->angle = 0;
  {
    s32 *owner = &gSequenceParentWork.currentOwner;
    /* FAKEMATCH: the mode pinned to r1 and read by an asm is set before the
     * owner is loaded; as a plain constant, the owner's load comes first (one
     * reordered row in tla-en). */
    register s32 mode asm("r1") = 16;

    __asm__("" : "+l"(owner) : "l"(mode));
    Actor_SetModeById(*owner, mode);
  }
  for (i = 0; i <= 7; i++) {
    Actor_SetPositionAndResetMotion(object, object->x, object->y,
                                     object->z + 0x30000);
    wait_(1);
  }
  FieldEvent_Wait_(20);
  party = &gSequenceParentWork;
  object->angle = 0x4000;
  /* FAKEMATCH: the barrier loads gSequenceParentWork's address among the angle's
   * stores and builds the owner's offset after them; as plain C, 4
   * instructions differ in tla-en. */
  asm volatile("" : "+l"(party));
  Func_020052d0(party->currentOwner, 258);
  Actor_SetModeById_(party->currentOwner, 27);
  FieldEvent_Wait_(30);
  Actor_SetModeById_(party->currentOwner, 28);
  FieldEvent_Wait_(10);
  Actor_SetSpritePriority_(party->currentOwner, 3);
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
    wait_(1);
    if (i == 10)
      Func_020053d8(204);
    if (i == 20) {
      ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
      Event_FinishScreenTransition();
    }
  }
  Func_020052f8(5);
}
