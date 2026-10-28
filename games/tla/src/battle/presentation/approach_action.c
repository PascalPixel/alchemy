#include "types.h"
#include "battle.h"
#include "battle_display.h"
#include "battle_obj.h"
#include "motobj.h"
#include "owner_state.h"
#include "ram_buffer.h"
#include "system.h"
#include "text/msg_ids.h"

void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY);
void Func_08138008(struct BattlePresentationWork *work);

/// The actor announces its attack, walks up to its target and plays the
/// attack's effect on it. Returns -1 when either id is not a battle object.
s32 BattlePres_RunApproachAction(struct BattleActionRecord *input) {
  struct BattlePresentationWork work;

  if (Ram_HeapSlots->transitionWork->targetYaw == 0x2000) {
    Ram_HeapSlots->transitionWork->targetYaw = 0x2000;
    WaitFrames(10);
  } else {
    Ram_HeapSlots->transitionWork->targetYaw = 0x2000;
    WaitFrames(30);
  }

  work.actor = input->unitId;
  if (BattleObject_IsValidId(work.actor) < 0)
    return -1;

  work.actors[0] = input->target;
  if (BattleObject_IsValidId(work.actors[0]) < 0)
    return -1;

  Unit_Get(work.actor);
  Unit_Get(work.actors[0]);
  Random16();
  MessageArgument_PushFar(work.actor, 1);
  Message_ShowAndWaitCoreFar((s32)MsgActorAttacks);
  BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
  Actor_SetAnimationSpeedFar(
      (struct DispatchObject *)BattleObject_Get(work.actor)->object, 16);
  BattleObject_Get(work.actors[0]);

  work.count = 1;
  if ((u16)work.actors[0] <= 7)
    work.side = 1;
  else
    work.side = 0;
  work.flags = 0;

  WaitFrames(4);
  Func_08138008(&work);
  ObjMotRst(work.actors[0]);
  ObjMotRst(work.actor);
  return 0;
}
