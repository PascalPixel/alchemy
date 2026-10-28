// Under -mthumb-slow-loads (26 October 2026): differ 2 of 180 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it, five minutes of permute; none reaches 0.
// Near miss, differ 2 of 180 (one reordered row): The Broken Seal's
// BattleStaging_RunUnitAction with its one-pass block, this game's heap slots and
// Actor_SetMode for the dispatch argument. In the reset loop the native stores
// both copies of the work pointer (str r2, [sp, #4]; str r2, [sp]) before it
// loads the actor (ldrsh r0, [r2, r6]); this draft loads between them. sched2:
// the second store is caller-save's spill of r2, inserted before the call and
// so after the load, and its alias set (106) differs from the load's (51), so
// no dependence orders them and their priorities (38, 37) keep RTL order.
// Measured: the block as plain statements (differ 103), the loop through a work
// pointer (65), nine minutes of permute, five on the loop alone (no gain).
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
  do {
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
  } while (0);
  for (i = 0; i != work.count; i++)
    ObjMotRst(work.actors[i]);
  ObjMotRst(work.actor);
  return 0;
}
