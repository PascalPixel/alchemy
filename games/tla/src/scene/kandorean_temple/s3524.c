#include "local.h"

void Func_02003524(void) {
  Func_020051c0();
  Func_02005370(0);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
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
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  u32 i;
  s32 delta;
  struct PartyState *party;

  Func_020051c0();
  Func_02005370(0);
  object->motionFlags = 0;
  ObjectMotion_SetActionVariantValue(gPartyState.currentOwner, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  object->angle = 0;
  {
    s32 *owner = &gPartyState.currentOwner;
    /* FAKEMATCH: the mode is pinned to r1 and read by an asm that passes the
     * owner's address through, so the mode is set before the owner is loaded as
     * the native has; plain, through the inline layer, value-returning or
     * pinned alone, the owner's load comes first (one reordered row). */
    register s32 mode asm("r1") = 16;

    __asm__("" : "+l"(owner) : "l"(mode));
    Object_SetModeById(*owner, mode);
  }
  for (i = 0; i <= 7; i++) {
    Object_SetPositionAndResetMotion(object, object->x, object->y,
                                     object->z + 0x30000);
    WaitFrames_(1);
  }
  FieldEvent_WaitFrames_(20);
  party = &gPartyState;
  object->angle = 0x4000;
  /* FAKEMATCH: the native loads the party state's address among the angle's
   * stores and builds the owner's offset after them, as overlay 671's
   * Func_02000a20 does; plain, the offset is built before the store (differ 4),
   * behind an empty asm after the store the address loads late (differ 2). */
  asm volatile("" : "+l"(party));
  Func_020052d0(party->currentOwner, 258);
  Object_SetModeById_(party->currentOwner, 27);
  FieldEvent_WaitFrames_(30);
  Object_SetModeById_(party->currentOwner, 28);
  FieldEvent_WaitFrames_(10);
  ObjectMotion_SetActionVariant_(party->currentOwner, 3);
  for (i = 0; i <= 39; i++) {
    {
      s32 scale = object->scaleX;

      /* FAKEMATCH: one variable set twice in the loop keeps both constants
       * inside it, as the native loads them on every pass and as overlay 671's
       * Func_02000a20 does; as plain constants loop.c moves -0x400 above the
       * loop (differ 8). */
      delta = -0x400;
      object->scaleX = scale + delta;
    }
    object->scaleY += delta;
    delta = -0x20000;
    object->y += delta;
    WaitFrames_(1);
    if (i == 10)
      Func_020053d8(204);
    if (i == 20) {
      ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
      Event_FinishScreenTransition();
    }
  }
  Func_020052f8(5);
}
