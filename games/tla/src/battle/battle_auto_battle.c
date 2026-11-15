/// @file
/// Auto battle: the actions it builds and plays without the command menu, the
/// shoulder-key camera and the escape.
// Part of battle.c.

#include "battle_internal.h"
#include "fixed_math.h"
#include "battle/battle.h"
#include "gamepak/far_call/battle_effect.h"
#include "gamepak/far_call/window.h"

/// Auto battle: gives every living unit a plain attack and sorts them into
/// action order. Returns how many actions there are.
s32 AutoBattle_BuildActions(struct BattleActionRecord *entries) {
  struct BattleActionRecord swap;
  u16 unitIds[UNIT_MAX];
  s32 partyCount;
  s32 count = 0;
  s32 enemyCount;
  s32 targetRange;
  s32 index;
  struct BattleActionRecord *entry;
  struct Unit *unit;

  // The party's actions. The four lookups' results are never used.
  partyCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
  for (index = 0; index != PARTY_BATTLE_SIZE; index++)
    Unit_Get(index);
  for (index = 0; index < partyCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry = &entries[index];
    entry->unitId = unitId;
    entry->priority = unit->agi;
    entry->command = COMMAND_ATTACK;
    entry->parameter = 0;
    entry->target = FIRST_ENEMY_UNIT;
    count++;
  }
  // The enemies attack a random party member, replaced when the action plays if
  // it is down.
  enemyCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
  entry = &entries[partyCount];
  targetRange = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, NULL);
  for (index = 0; index < enemyCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry->unitId = unitId;
    entry->priority = unit->agi >> 1;
    if (entry->priority != 0)
      entry->priority += (u32)(Random16() * unit->agi) >> 16;
    entry->command = COMMAND_ATTACK;
    entry->parameter = 0;
    entry->target = Random_Below(targetRange);
    count++;
    entry++;
  }
  // Bubble the faster units forward.
  // BUG: one pass short of a full sort; with two actions nothing is sorted.
  for (index = count - 2; index > 0; index--) {
    s32 swaps = 0;
    s32 pos;

    for (pos = count - 1; pos > 0; pos--) {
      if ((s16)entries[pos].priority > (s16)entries[pos - 1].priority) {
        Dma_Set(&entries[pos], &swap, DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&entries[pos - 1], &entries[pos], DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&swap, &entries[pos - 1], DMA_COPY_WORDS(4), REG_DMA3);
        swaps++;
      }
    }
    if (swaps == 0)
      break;
  }
  return count;
}

/// Turns the battle camera while L or R is held, a 128th of a turn each
/// frame, and redraws the zoomed background unless a transition
/// holds it.
void BattleCamera_AdjustByShoulderKeys(void) {
  struct BattleCamera *cam = Ram_HeapSlots->cameraWork;
  struct BattleBackgroundWork *trans = Ram_HeapSlots->battleBackground;

  if ((gInput.held & KEY_L) != 0) {
    cam->yaw += TRIG_TURN / 128;
  }
  if ((gInput.held & KEY_R) != 0) {
    cam->yaw -= TRIG_TURN / 128;
  }
  if (trans->flag == 0) {
    BattleCamera_SetRange(toFIXED(120), toFIXED(120), 0, 0, FIXED_ONE);
  }
}

/// Auto battle: plays one action, a defeated target replaced by a living unit
/// of its side. Returns 1 when the party got away, -1 when the unit is down.
s32 AutoBattle_RunAction(struct BattleActionRecord *action) {
  struct BattleBackgroundWork *transition;
  s32 unitId;
  s32 yaw;
  struct Unit *actor;

  unitId = action->unitId;
  actor = Unit_Get(unitId);
  if (actor->hp == 0)
    return -1;

  action->target = BattleAbility_ReplaceDefeated(action);
  transition = Ram_HeapSlots->battleBackground;
  // Here any unit id above 4 counts as an enemy.
  if (action->unitId > 4)
    yaw = -0x2000;
  else
    yaw = YAW_PARTY_ACTION;
  transition->targetYaw = yaw;
  transition->frames = 60;
  MessageArgument_ClearAllFar();

  switch (action->command) {
  case COMMAND_FLEE:
    BattleMessage_ShowAndWaitFar((s32)&MsgPartyFlees);
    if (BattleFlee_PlayRun(action) != 0)
      return 1;
    break;
  case COMMAND_DEFEND:
    wait(45);
    AutoBattle_ShowDefend((s16 *)action);
    break;
  case COMMAND_ITEM:
    wait(45);
    AutoBattle_RunAttackOrItem(action);
    break;
  case COMMAND_ATTACK:
  default: {
    struct BattleBackgroundWork *view = Ram_HeapSlots->battleBackground;
    view->flag = 0;
    AutoBattle_RunAttackOrItem(action);
    view->flag = 0;
    break;
  }
  case COMMAND_PSYNERGY:
    AutoBattle_RunPsynergy(action);
    break;
  }

  Message_ResetIdleFar();
  return 0;
}

/// Returns 0 for a party member's or an enemy's unit id, -1 for any other.
s32 BattleObject_IsValidId(u32 unitId) {
  if (unitId <= ALLY_MAX) {
    return 0;
  }
  if (unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE) {
    return 0;
  }
  return -1;
}

/// Auto battle: the party tries to run, and gets away 15 times in 16. Returns 1
/// when it got away.
s32 BattleFlee_PlayRun(struct BattleActionRecord *action) {
  s16 partyMembers[UNIT_MAX];
  s32 partySize;
  s32 memberSlot;

  (void)action;
  // The roll's top four bits: flee unless all are clear, 15 in 16.
  if (((Random16() << 4) >> 16) != 0) {
    partySize = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, partyMembers);
    for (memberSlot = 0; memberSlot != partySize; memberSlot++) {
      BattleMotion_SetupEscapeObject(partyMembers[memberSlot]);
      wait(8);
    }
    wait(22);
    return 1;
  }
  BattleMessage_ShowAndWaitFar(MsgCannotEscape);
  return 0;
}

/// Auto battle: shows that a standing unit is defending. The message is
/// `<unit> is defending`. Returns -1 for an invalid unit id, 0 otherwise.
s32 AutoBattle_ShowDefend(s16 *script) {
  s32 objectId;
  s32 result;
  struct Unit *unit;

  objectId = *script;
  unit = Unit_Get(objectId);
  if (BattleObject_IsValidId(objectId) < 0) {
    return -1;
  }
  result = 0;
  if (unit->hp <= 0) {
    return result;
  }
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(objectId, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar((s32)MsgActorDefends);
  return 0;
}

/// Auto battle: casts Psynergy. The camera turns to the caster's side and
/// the caster raises its hand; the target side fades out, its living units
/// are readied alone, the Psynergy's effect (kind action->parameter) plays
/// on them, and everyone fades back in. Returns -1 for an invalid unit or
/// target. Called by AutoBattle_RunAction for COMMAND_PSYNERGY. Writes
/// REG_BLDCNT and REG_BLDALPHA and the actors' colour modes, and leaves the
/// battle objects readied again.
s32 AutoBattle_RunPsynergy(struct BattleActionRecord *action) {
  u16 visibleUnits[UNIT_MAX];
  struct BattlePresentationWork context;
  s32 actorId;
  s32 targetId;
  u32 sideStart;
  u32 sideCount;
  u32 livingCount;
  struct MotionObject *actorRecord;
  u32 index;

  livingCount = 0;
  actorId = action->unitId;
  if (BattleObject_IsValidId(actorId) < 0)
    return -1;
  targetId = action->target;
  if (BattleObject_IsValidId(targetId) < 0)
    return -1;

  {
    struct BattleBackgroundWork *transition = Ram_HeapSlots->battleBackground;
    transition->targetYaw =
        action->unitId > 4 ? YAW_BEHIND_ENEMIES : YAW_PARTY_ACTION;
    transition->frames = 60;
  }
  wait(10);
  Random16();
  actorRecord = BattleObject_Get(actorId)->object;

  if (targetId >= 0 && targetId <= ALLY_MAX) {
    sideCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
    sideStart = FIRST_ENEMY_UNIT;
  } else {
    sideCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
    sideStart = 0;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == actorId)
      Actor_SelectAnimationFar((struct Actor *)actorRecord, POSE_CAST);
  }

  wait(30);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  for (index = 0; index != BLEND_WEIGHT_MAX; index++) {
    REG_BLDALPHA = (BLEND_WEIGHT_MAX - index) | 0x1000;
    wait(1);
  }
  PartyStatus_DrawFar(PARTY_STATUS_CLOSED);

  if (targetId > (FIRST_ENEMY_UNIT - 1)) {
    u32 count = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      u32 unit = index + FIRST_ENEMY_UNIT;
      if (Unit_Get(unit)->hp > 0)
        visibleUnits[livingCount++] = unit;
    }
  } else {
    u32 count = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      if (Unit_Get(index)->hp > 0)
        visibleUnits[livingCount++] = index;
    }
  }
  visibleUnits[livingCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForList((s16 *)visibleUnits, 0);

  context.kind = action->parameter;
  context.actor = actorId;
  for (index = 0; index != livingCount; index++)
    context.actors[index] = visibleUnits[index];
  context.count = livingCount;
  if (targetId >= 0 && targetId <= ALLY_MAX)
    context.side = EFFECT_SIDE_PARTY;
  else
    context.side = EFFECT_SIDE_ENEMIES;
  BattleEffect_RunSummonFar((s32 *)&context);

  wait(10);
  BattleObject_Put();
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  for (index = 0; index != BLEND_WEIGHT_MAX; index++) {
    REG_BLDALPHA = index | 0x1000;
    wait(1);
  }
  for (index = 0; index != sideCount; index++)
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_OPAQUE);
  BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
  wait(3);
  return 0;
}

/// The actor announces its attack, walks up to its target and plays the
/// attack's effect on it. Returns -1 when either id is not a battle object.
s32 AutoBattle_RunApproachAttack(struct BattleActionRecord *input) {
  struct BattlePresentationWork work;

  if (Ram_HeapSlots->battleBackground->targetYaw == YAW_PARTY_ACTION) {
    Ram_HeapSlots->battleBackground->targetYaw = YAW_PARTY_ACTION;
    wait(10);
  } else {
    Ram_HeapSlots->battleBackground->targetYaw = YAW_PARTY_ACTION;
    wait(30);
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
  MessageArgument_PushFar(work.actor, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar((s32)MsgActorAttacks);
  BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
  Actor_SetAnimationSpeedFar(
      (struct Actor *)BattleObject_Get(work.actor)->object, 16);
  BattleObject_Get(work.actors[0]);

  work.count = 1;
  if ((u16)work.actors[0] <= ALLY_MAX)
    work.side = EFFECT_SIDE_PARTY;
  else
    work.side = EFFECT_SIDE_ENEMIES;
  work.flags = 0;

  wait(4);
  BattleEffect_RunByIdRangeFar(&work);
  BattleMotion_ReturnToSpot(work.actors[0]);
  BattleMotion_ReturnToSpot(work.actor);
  return 0;
}

/// Auto battle: an attack or an item. Turns the camera to the acting unit's
/// side, poses the unit and plays the battle effect (kind action->parameter) on
/// the first living unit of the target's side, then sends both back. Returns -1
/// for an invalid unit. Called by AutoBattle_RunAction for
/// COMMAND_ATTACK, COMMAND_ITEM and any other command but fleeing, defending
/// and Psynergy.
s32 AutoBattle_RunAttackOrItem(struct BattleActionRecord *action) {
  struct BattlePresentationWork work;
  struct BattleBackgroundWork *transition = Ram_HeapSlots->battleBackground;
  struct MotionObject *object;
  s32 facing;
  s32 i;

  if (action->unitId > 4)
    facing = -DEGtoANG(45);
  else
    facing = DEGtoANG(45);
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
  if (work.firstTarget > (FIRST_ENEMY_UNIT - 1))
    work.count =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, (u16 *)work.actors);
  else
    work.count =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)work.actors);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus &
                      ~PARTY_STATUS_SHOW_PP);
  object = BattleObject_Get(work.actor)->object;
  Actor_SelectAnimationFar((struct Actor *)object, POSE_CAST);
  Actor_SetAnimationSpeedFar((struct Actor *)object, 16);
  if ((u16)action->target <= ALLY_MAX) {
    work.side = EFFECT_SIDE_PARTY;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)work.actors);
    work.count = 1;
  } else {
    work.side = EFFECT_SIDE_ENEMIES;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, (u16 *)work.actors);
    work.count = 1;
  }
  for (i = 0; i != work.count; i++) {
    struct Sprite *animation =
        (struct Sprite *)BattleObject_Get(work.actors[i])->object->records;
    s32 j;

    for (j = 0; j != animation->count - 1; j++)
      work.params[i][j] = animation->entries[j]->colorMode;
  }
  work.kind = ELEMENT_VENUS;
  work.variant = 0;
  BattleEffect_RunElementPillarFar(&work);
  work.kind = ELEMENT_MERCURY;
  BattleEffect_RunElementPillarFar(&work);
  work.kind = ELEMENT_MARS;
  BattleEffect_RunElementPillarFar(&work);
  work.kind = ELEMENT_JUPITER;
  BattleEffect_RunElementPillarFar(&work);
  work.kind = 0;
  BattleEffect_RunFromTableFar(&work);
  Actor_SelectAnimationFar((struct Actor *)object, POSE_STANDING);
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);
  for (i = 0; i != work.count; i++)
    BattleMotion_ReturnToSpot(work.actors[i]);
  BattleMotion_ReturnToSpot(work.actor);
  return 0;
}

/// Unused auto battle damage: takes damage HP from a unit and shows it, with "A
/// bitter blow!" or "A critical hit!" first when showMessage is set.
void AutoBattle_ApplyDamage(u32 unitId, s32 damage, s32 showMessage,
                            u8 *context) {
  u8 localContext[4];
  struct Unit *character;
  struct BattleObject *slot;

  if (context == 0) {
    context = localContext;
    context[0] = 0;
    context[1] = 0;
    context[2] = 0;
    context[3] = 0;
  }

  character = Unit_Get(unitId);
  character->hp -= damage;
  if (character->hp < 0)
    character->hp = 0;

  slot = BattleObject_Get(unitId);
  Actor_SelectAnimationFar((struct Actor *)slot->object, POSE_DAMAGE);
  PartyStatus_DrawFar(PARTY_STATUS_HP_ONLY);
  MessageArgument_ClearAllFar();

  if (unitId <= ALLY_MAX) {
    if (showMessage != 0)
      BattleMessage_ShowAndWaitFar((s32)MsgBitterBlow);
    MessageArgument_PushFar(damage, MESSAGE_ARGUMENT_NUMBER);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)MsgDmgP);
  } else {
    if (showMessage != 0)
      BattleMessage_ShowAndWaitFar((s32)MsgCritical);
    MessageArgument_PushFar(damage, MESSAGE_ARGUMENT_NUMBER);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)MsgDmgE);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  }

  BattleMotion_PlayFall(unitId);
  if (unitId <= ALLY_MAX) {
    if (character->hp <= 0) {
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)MsgGoesDown);
    }
  } else if (character->hp <= 0) {
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)MsgDowned);
  }

  slot = BattleObject_Get(unitId);
  Actor_SelectAnimationFar((struct Actor *)slot->object, POSE_STANDING);
}

/// For a unit whose HP has run out: plays its falling effect to the last frame
/// and marks its battle object for release (BattleObject_MarkForRelease).
void BattleMotion_PlayFall(s32 unitId) {
  struct Sprite *animation;
  struct SpriteAnimation *entry;

  if (Unit_Get(unitId)->hp <= 0) {
    animation = BattleObject_Get(unitId)->object->records;
    Sprite_StartAnimationFar(animation, POSE_DAMAGE);
    entry = animation->entries[0];
    entry->colorMode = SPRITE_COLOR_GREY;
    entry->picture = 0xFF;
    wait(4);
    SpriteDissolve_RunFar(animation);
    BattleObject_MarkForRelease(unitId);
  }
}

/// BattleAbility_ReplaceDefeated's result when the target's side is all down,
/// which the battle's end comes before.
#define TARGET_SIDE_DEFEATED 256

/// Returns the action's target, or a random living unit of its side when the
/// target is down; 0x100 when none is left.
s32 BattleAbility_ReplaceDefeated(const struct BattleActionRecord *action) {
  s16 livingUnits[UNIT_MAX];
  s32 targetId;
  s32 livingCount;

  targetId = action->target;
  if (Unit_Get(targetId)->hp != 0) {
    return targetId;
  }

  if (targetId > (FIRST_ENEMY_UNIT - 1)) {
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
  } else {
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
  }

  if (livingCount == 0) {
    return TARGET_SIDE_DEFEATED;
  }

  return livingUnits[(u32)(Random16() * livingCount) >> 16];
}
