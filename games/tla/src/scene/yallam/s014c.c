#include "yallam.h"
#include "edition.h"

// Japanese has no stop beside the leader: its function stays on the
// overlay's scaffold, with a draft.
#if (LANGUAGE != LANGUAGE_JAPANESE)
/// Stops the object (mode 2) when the leader is within 11 of it; otherwise
/// lets it face object 10 or 9 by bit 0 of its action word and, failing that,
/// the leader, forced when bit 1 is set and event word 352 or tile byte 4 is.
s32 Func_0200014c(struct ObjectRuntime *object) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Adventure *party = &gAdventure;
  struct ObjectRuntime *leader;
  s32 force = 0;

  /* FAKEMATCH(barrier): holding the adventure's address with r2 clobbered
   * loads it before the owner offset is built; as Object_GetById(gAdventure.
   * currentOwner), 4 of 82 instructions differ in tla-en. */
  asm("" : "+l"(party) : : "r2");
  leader = Object_GetById(party->currentOwner);

  if (SceneActor_CalculateFixedPointDistance(&leader->x, &object->x) <= 11) {
    u8 *state = &object->movementState;
    /* FAKEMATCH(temporary): the call's argument copied before the barrier
     * below; as object->movementState = force; Func_02002448(object, 2), 4 of
     * 82 instructions differ in tla-en. */
    struct ObjectRuntime *arg = object;

    /* FAKEMATCH(barrier): the state's address and the argument are both
     * formed before the state is stored, as the native does; as above. */
    asm("" : : "l"(arg), "l"(state));
    *state = force;
    Func_02002448(arg, 2);
  } else {
    struct ObjectRuntime *target;
    u16 *flags = (u16 *)&object->action;

    if (*flags & 1)
      target = Object_GetById(10);
    else
      target = Object_GetById(9);
    if (Func_02000088(object, target, 24, 0) == 0) {
      leader = Object_GetById(gAdventure.currentOwner);
      if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
        force = 1;
      Func_02000088(object, leader, 24, force);
    }
  }
  return 0;
}
#endif
