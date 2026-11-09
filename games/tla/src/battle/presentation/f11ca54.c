#include "types.h"
#include "sprite.h"
#include "actor_view.h"
#include "battle.h"
#include "battle_display.h"
#include "battle_side.h"
#include "battle_obj.h"
#include "battle_work.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "system.h"

void Actor_SetMode(struct MotionObject *object, s32 mode);
void Func_08138018(struct BattlePresentationWork *work);
void Func_08138020(struct BattlePresentationWork *work);

s32 Func_0811ca54(struct BattleActionRecord *action) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct MotionObject *object;
  s32 facing;
  s32 i;

  if (action->unitId > 4)
    facing = -0x2000;
  else
    facing = 0x2000;
  if (transition->targetYaw == facing) {
    transition->frames = 40;
    wait(40);
  } else {
    transition->targetYaw = facing;
    transition->frames = 40;
    wait(40);
  }
  work.kind = action->parameter;
  work.initialValue = action->range;
  work.actor = action->unitId;
  work.firstTarget = action->target;
  if (BattleObject_IsValidId(work.actor) < 0)
    return -1;
  if (work.firstTarget > 127)
    work.count = BattleParty_ListLivingUnits(2, (u16 *)work.actors);
  else
    work.count = BattleParty_ListLivingUnits(1, (u16 *)work.actors);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  object = BattleObject_Get(work.actor)->object;
  Actor_SetMode(object, 3);
  Actor_SetAnimationSpeedFar((struct ActorView *)object, 16);
  if ((u16)action->target <= 7) {
    work.side = 1;
    BattleParty_ListLivingUnits(1, (u16 *)work.actors);
    work.count = 1;
  } else {
    work.side = 0;
    BattleParty_ListLivingUnits(2, (u16 *)work.actors);
    work.count = 1;
  }
  for (i = 0; i != work.count; i++) {
    struct ActorSpriteView *animation =
        BattleObject_Get(work.actors[i])->object->records;
    s32 j;

    for (j = 0; j != animation->count - 1; j++)
      work.params[i][j] = animation->entries[j]->param;
  }
  work.kind = 0;
  work.variant = 0;
  Func_08138020(&work);
  work.kind = 1;
  Func_08138020(&work);
  work.kind = 2;
  Func_08138020(&work);
  work.kind = 3;
  Func_08138020(&work);
  work.kind = 0;
  Func_08138018(&work);
  Actor_SetMode(object, 1);
  // An empty once-only loop ends CSE's knowledge of work's address, so the
  // return loop's exit test addresses work afresh; without it, 103 of 180
  // instructions differ in tla-en.
  do {
  } while (0);
  for (i = 0; i != work.count; i++)
    ObjMotRst(work.actors[i]);
  ObjMotRst(work.actor);
  return 0;
}
