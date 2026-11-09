#include "types.h"
#include "version.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"

// The Japanese form of this function, the older revision without the test
// that the leader stands within 11 of the actor, is still on its listing.
#if TLA_PATCH_1

s32 SceneActor_CalculateFixedPointDistance(s32 *from, s32 *to);
s32 Func_020002d8(struct Actor *actor, struct Actor *target, s32 distance,
                  s32 flag);
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

  target = Actor_Get(gMainRoutineWork.currentOwner);
  if (SceneActor_CalculateFixedPointDistance(&target->x, &actor->x) <= 11) {
    actor->movementState = flag;
    Func_020043c4(actor, 2);
    return 0;
  }
  if (actor->action == 0)
    target = Actor_Get(18);
  else if (actor->action == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (actor->action & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}

#else

s32 Func_020002d8(struct Actor *actor, struct Actor *target, s32 distance,
                  s32 flag);

s32 Func_020003bc(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *work = Ram_HeapSlots->eventWork;
  s16 *action = &actor->action;
  s32 flag = 0;
  struct Actor *target;

  if (*action == 0)
    target = Actor_Get(18);
  else if (*action == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (*action & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}

#endif
