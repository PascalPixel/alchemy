#include "types.h"
#include "edition.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

// Japanese keeps its own form of this function, still in its listing: it
// lacks the test that the leader stands within 11 of the actor.
#if LANGUAGE != LANGUAGE_JAPANESE

struct Actor *Actor_GetById(s32 id);
s32 SceneActor_CalculateFixedPointDistance(s32 *from, s32 *to);
s32 Func_020002d8(struct Actor *actor, struct Actor *target,
                  s32 distance, s32 flag);
void Func_020043c4(struct Actor *actor, s32 mode);

/// Once the leader is within 11 of the actor, stops it (Func_020043c4 mode
/// 2). Otherwise it runs Func_020002d8 toward object 18, 16 or 17 by its
/// action (0, 1, other) at 24, and if that returns 0, toward the leader at
/// 32, flagged when action bit 1 is set while the event work's halfword
/// 0x160 or byte 4 of the window tiles is nonzero.
s32 Func_020003bc(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *work = Ram_HeapSlots->eventWork;
  s32 flag = 0;
  struct Actor *target;

  {
    /* FAKEMATCH(pin): gMainRoutineWork's address pinned to r3 loads from the
     * pool before the leader's offset, as native; as
     * Actor_GetById(gMainRoutineWork.currentOwner), 2 of 85 instructions differ in
     * tla-en. */
    register struct MainRoutineWork *party asm("r3") = &gMainRoutineWork;

    /* FAKEMATCH(barrier): the barrier of the pin above: it holds r2 until the
     * address is loaded. */
    __asm__("" : "+l"(party) : : "r2");
    target = Actor_GetById(party->currentOwner);
  }
  if (SceneActor_CalculateFixedPointDistance(&target->x, &actor->x) <= 11) {
    actor->movementState = flag;
    Func_020043c4(actor, 2);
    return 0;
  }
  if (actor->action == 0)
    target = Actor_GetById(18);
  else if (actor->action == 1)
    target = Actor_GetById(16);
  else
    target = Actor_GetById(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Actor_GetById(gMainRoutineWork.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (actor->action & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}

#endif
