/// @file
/// Auto battle: the actions it builds and plays, without the command menu.
// Part of battle.c.

#include "../battle_internal.h"

/// Auto battle: gives every living unit a plain attack and sorts them into
/// turn order. The party act at their Agility and target the first enemy
/// unit, 128; enemies act at half their Agility plus a random part of it and
/// target unit id 0 to n - 1 for n living party members, which
/// BattlePresentation_RunAction replaces if it is down. Returns how many
/// actions there are. Called by Battle_RunEncounter each round under
/// FLAG_AUTO_BATTLE; fills entries.
s32 BattlePresentation_BuildSortedUnitEntries(
    struct BattleActionRecord *entries) {
  struct BattleActionRecord swap;
  u16 unitIds[14];
  s32 partyCount;
  s32 count = 0;
  s32 enemyCount;
  s32 targetRange;
  s32 index;
  struct BattleActionRecord *entry;
  struct BattleUnit *unit;

  // The party's actions. The four lookups' results are never used.
  partyCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
  for (index = 0; index != PARTY_BATTLE_SIZE; index++)
    UnitGetFar(index);
  for (index = 0; index < partyCount; index++) {
    s32 unitId = unitIds[index];

    unit = UnitGetFar(unitId);
    entry = &entries[index];
    entry->unitId = unitId;
    entry->priority = unit->agi;
    entry->command = COMMAND_ATTACK;
    entry->parameter = 0;
    entry->target = FIRST_ENEMY_UNIT;
    count++;
  }
  // The enemies' actions. Random16() * n >> 16 is a roll from 0 to n - 1;
  // the target is that number taken as a unit id, which
  // BattlePresentation_RunAction replaces if the unit is down.
  enemyCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unitIds);
  entry = &entries[partyCount];
  targetRange = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
  for (index = 0; index < enemyCount; index++) {
    s32 unitId = unitIds[index];

    unit = UnitGetFar(unitId);
    entry->unitId = unitId;
    entry->priority = unit->agi >> 1;
    if (entry->priority != 0)
      entry->priority += (Random16() * unit->agi) >> 16;
    entry->command = COMMAND_ATTACK;
    entry->parameter = 0;
    entry->target = (Random16() * targetRange) >> 16;
    count++;
    entry++;
  }

  // Bubble the faster units forward until a pass swaps nothing.
  // BUG: count - 2 passes at most, one short of a full sort; with two
  // actions nothing is sorted.
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

/// Turns the camera while L or R is held. Nothing in the source calls it.
DEFINE_ADJUST_CAMERA(BattlePresentation_AdjustCameraByShoulderKeys)

/// Auto battle: plays one action. A unit already down does nothing (-1);
/// a defeated target is replaced by a living unit of its side first.
/// Returns 1 when the party got away, 0 otherwise. Called by
/// Battle_RunEncounter for each action under FLAG_AUTO_BATTLE. Turns the
/// camera to the actor's side (gTransitionWork) and rewrites action->target.
s32 BattlePresentation_RunAction(struct BattleActionRecord *action) {
  struct BattlePresentationTransition *transition;
  s32 actorId;
  s32 yaw;
  struct BattleUnit *actor;

  actorId = action->unitId;
  actor = UnitGetFar(actorId);
  if (actor->hp == 0)
    return -1;

  action->target = BattleTarget_ReplaceDefeated(action);
  transition = gTransitionWork;
  // Here any unit id above 4 counts as an enemy, Jenna's 5 included
  // (character_ids.h).
  if (action->unitId > 4)
    yaw = YAW_ENEMY_TURN;
  else
    yaw = YAW_PARTY_TURN;
  transition->targetYaw = yaw;
  transition->frames = 60;
  UiWork_ClearValueNameTablesFar();

  switch (action->command) {
  case COMMAND_FLEE:
    UiText_ShowMessageAndWaitCoreFar(MsgPartyFlees);
    if (BattleEscape_PlayRun(action) != 0)
      return 1;
    break;
  case COMMAND_DEFEND:
    WaitFrames(45);
    BattlePresentation_ShowDefendMessage(action);
    break;
  case COMMAND_ITEM:
    WaitFrames(45);
    BattlePresentation_RunUnitAction(action);
    break;
  case COMMAND_ATTACK:
  default: {
    struct BattlePresentationTransition *transition = gTransitionWork;
    transition->flag = 0;
    BattlePresentation_RunUnitAction(action);
    transition->flag = 0;
    break;
  }
  case COMMAND_PSYNERGY:
    BattlePresentation_RunPairedUnitTransition(action);
    break;
  }

  UiWork_ResetFreeChannelFar();
  return 0;
}

/// Returns 0 for a party member's or an enemy's unit id, -1 for any other.
/// Called by the auto battle's action players.
s32 BattleObject_IsValidId(u32 unitId) {
  if (unitId <= ALLY_MAX) {
    return 0;
  }
  if (unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE) {
    return 0;
  }
  return -1;
}

/// Auto battle: the party tries to run, and gets away 15 times in 16, each
/// living member running off in turn. Returns 1 when they got away; else
/// shows MsgCannotEscape and returns 0. Called by BattlePresentation_RunAction.
s32 BattleEscape_PlayRun(struct BattleActionRecord *action) {
  s16 partyMembers[14];
  s32 partySize;
  s32 animated;
  s32 memberSlot;

  (void)action;
  // The roll's top four bits: escape unless all are clear, 15 in 16.
  if (((Random16() << 4) >> 16) != 0) {
    partySize = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, partyMembers);
    animated = 0;
    if (partySize != 0) {
      memberSlot = 0;
      do {
        BattleMotion_SetupEscapeObject(partyMembers[memberSlot]);
        animated++;
        WaitFrames(8);
        memberSlot++;
      } while (animated != partySize);
    }
    WaitFrames(22);
    return 1;
  }
  UiText_ShowMessageAndWaitCoreFar(MsgCannotEscape);
  return 0;
}

/// Auto battle: shows that a standing unit is defending. The message is
/// `<unit> is defending`. Returns -1 for an invalid unit id, 0 otherwise.
/// Called by BattlePresentation_RunAction.
s32 BattlePresentation_ShowDefendMessage(
    const struct BattleActionRecord *action) {
  s32 unitId;
  s32 result;
  struct BattleUnit *unit;

  unitId = action->unitId;
  unit = UnitGetFar(unitId);
  if (BattleObject_IsValidId(unitId) < 0) {
    return -1;
  }
  result = 0;
  if (unit->hp <= 0) {
    return result;
  }
  UiWork_ClearValueNameTablesFar();
  UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
  UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
  return 0;
}

/// Auto battle: casts Psynergy. The camera turns to the caster's side and
/// the caster raises its hand; the target side fades out, its living units
/// are readied alone, the Psynergy's effect (kind action->parameter) plays
/// on them, and everyone fades back in. Returns -1 for an invalid actor or
/// target. Called by BattlePresentation_RunAction for COMMAND_PSYNERGY. Writes
/// REG_BLDCNT and REG_BLDALPHA and the actors' record modes, and leaves the
/// battle objects readied again.
s32 BattlePresentation_RunPairedUnitTransition(
    struct BattleActionRecord *action) {
  u16 visibleUnits[14];
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
  if (BattleObject_IsValidId(actorId) < 0) {
    return -1;
  }
  targetId = action->target;
  if (BattleObject_IsValidId(targetId) < 0) {
    return -1;
  }

  {
    struct BattlePresentationTransition *transition = gTransitionWork;
    // An enemy caster turns the camera to YAW_BEHIND_ENEMIES, not
    // YAW_ENEMY_TURN.
    transition->targetYaw =
        action->unitId > 4 ? YAW_BEHIND_ENEMIES : YAW_PARTY_TURN;
    transition->frames = 60;
  }
  WaitFrames(10);
  // A roll nothing uses, which still advances the random sequence.
  Random16();
  actorRecord = BattleObjGet(actorId)->object;

  // The caster's side, taken as the side the target is not on, and the
  // caster's POSE_CAST when its id is among that side's first sideCount ids.
  if (targetId >= 0 && targetId <= ALLY_MAX) {
    sideCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, visibleUnits);
    sideStart = FIRST_ENEMY_UNIT;
  } else {
    sideCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, visibleUnits);
    sideStart = 0;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == actorId) {
      ObjectDispatch_SelectAnimationFar(actorRecord, POSE_CAST);
    }
  }

  // Fade the caster's side out: its sprites ACTOR_SEMI_TRANSPARENT,
  // their blend weight falling from 16 to 1 over 16 frames. Then the party
  // status window is erased (PARTY_STATUS_CLOSED).
  WaitFrames(30);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattlePresentation_SetActorRecordMode(index + sideStart,
                                          ACTOR_SEMI_TRANSPARENT);
  }
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = BLEND_ALPHA(16 - index, 16);
    WaitFrames(1);
  }
  PartyStatusDrawFar(PARTY_STATUS_CLOSED);

  // Ready only the target side's living units, and play the effect. The
  // walk counts unit ids up from the side's first until it has found as
  // many living units as the side has.
  if (targetId > FIRST_ENEMY_UNIT - 1) {
    u32 count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      u32 unit = index + FIRST_ENEMY_UNIT;
      if (UnitGetFar(unit)->hp > 0) {
        visibleUnits[livingCount++] = unit;
      }
    }
  } else {
    u32 count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      if (UnitGetFar(index)->hp > 0) {
        visibleUnits[livingCount++] = index;
      }
    }
  }
  visibleUnits[livingCount] = BATTLE_UNIT_LIST_END;
  BattleObject_SpawnObjectsForList(visibleUnits, 0);

  context.effect.kind = action->parameter;
  context.effect.actor = actorId;
  for (index = 0; index != livingCount; index++) {
    context.effect.actors[index] = visibleUnits[index];
  }
  context.effect.count = livingCount;
  if (targetId >= 0 && targetId <= ALLY_MAX) {
    context.effect.side = EFFECT_SIDE_PARTY;
  } else {
    context.effect.side = EFFECT_SIDE_ENEMIES;
  }
  BattleEffect_RunSceneEffectFar(&context.effect);

  // Bring every unit back and fade the caster's side in.
  WaitFrames(10);
  BattleObjPut();
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattlePresentation_SetActorRecordMode(index + sideStart,
                                          ACTOR_SEMI_TRANSPARENT);
  }
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = BLEND_ALPHA(index, 16);
    WaitFrames(1);
  }
  for (index = 0; index != sideCount; index++) {
    BattlePresentation_SetActorRecordMode(index + sideStart, ACTOR_OPAQUE);
  }
  BattlePresentation_SetupTransitionScene(0, 0, 0, 0x64);
  WaitFrames(3);
  return 0;
}

/// Auto battle: an attack. Shows `<unit> attacks!`, sends the attacker up
/// to its target, plays the blow and sends both back. Returns -1 for an
/// invalid attacker or target. Nothing in the source calls it.
s32 BattlePresentation_RunApproachAction(struct BattleActionRecord *input) {
  struct BattlePresentationWork work;

  // A short wait when the camera already faces the party's turn.
  if (gTransitionWork->targetYaw == YAW_PARTY_TURN) {
    gTransitionWork->targetYaw = YAW_PARTY_TURN;
    WaitFrames(10);
  } else {
    gTransitionWork->targetYaw = YAW_PARTY_TURN;
    WaitFrames(30);
  }

  work.effect.actor = input->unitId;
  if (BattleObject_IsValidId(work.effect.actor) < 0)
    return -1;

  work.effect.actors[0] = input->target;
  if (BattleObject_IsValidId(work.effect.actors[0]) < 0)
    return -1;

  // Lookups whose results are unused, and a roll that only advances the
  // random sequence.
  UnitGetFar(work.effect.actor);
  UnitGetFar(work.effect.actors[0]);
  Random16();
  UiWork_PushValueSlotFar(work.effect.actor, VALUE_UNIT_NAME);
  UiText_ShowMessageAndWaitCoreFar(MsgActorAttacks);
  BattleMotion_ApproachTarget(work.effect.actor, work.effect.actors[0], 13, 0);
  ObjectDispatch_SetAnimationSpeedFar(
      (struct DispatchObject *)BattleObjGet(work.effect.actor)->object, 16);
  BattleObjGet(work.effect.actors[0]);

  work.effect.count = 1;
  if ((u16)work.effect.actors[0] <= ALLY_MAX)
    work.effect.side = EFFECT_SIDE_PARTY;
  else
    work.effect.side = EFFECT_SIDE_ENEMIES;
  work.effect.prelude = 0;

  WaitFrames(4);
  BattleEffect_DispatchByIdRangeFar(&work.effect);
  ObjMotRst(work.effect.actors[0]);
  ObjMotRst(work.effect.actor);
  return 0;
}

/// Auto battle: an attack or an item. Turns the camera to the actor's side,
/// poses the actor and plays the effect (kind action->parameter) on the
/// first living unit of the target's side, then sends both back. Returns -1
/// for an invalid actor. Called by BattlePresentation_RunAction for
/// COMMAND_ATTACK, COMMAND_ITEM and any other command but fleeing, defending
/// and Psynergy.
s32 BattlePresentation_RunUnitAction(struct BattleActionRecord *action) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition = gTransitionWork;
  struct MotionObject *object;
  s32 targetYaw;
  s32 targetIndex;

  if (action->unitId > 4)
    targetYaw = YAW_ENEMY_TURN;
  else
    targetYaw = YAW_PARTY_TURN;
  if (transition->targetYaw == targetYaw) {
    transition->frames = 40;
    WaitFrames(40);
  } else {
    transition->targetYaw = targetYaw;
    transition->frames = 40;
    WaitFrames(40);
  }
  work.effect.kind = action->parameter;
  work.effect.range = action->range;
  work.effect.actor = action->unitId;
  work.effect.firstTarget = action->target;
  if (BattleObject_IsValidId(work.effect.actor) < 0)
    return -1;
  if (work.effect.firstTarget > FIRST_ENEMY_UNIT - 1)
    work.effect.count =
        BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, work.effect.actors);
  else
    work.effect.count =
        BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, work.effect.actors);
  // The status window without its PP row (see PARTY_STATUS_SUMMONS).
  PartyStatusDrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);
  // The actor raises its hand (POSE_CAST); then only the first living unit of
  // the target's side takes the effect.
  object = BattleObjGet(work.effect.actor)->object;
  ObjectDispatch_SelectAnimationFar(object, POSE_CAST);
  ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, 16);
  if ((u16)action->target <= ALLY_MAX) {
    work.effect.side = EFFECT_SIDE_PARTY;
    BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, work.effect.actors);
    work.effect.count = 1;
  } else {
    work.effect.side = EFFECT_SIDE_ENEMIES;
    BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, work.effect.actors);
    work.effect.count = 1;
  }

  // Keep each target's sprite entries' parameters for the effect.
  for (targetIndex = 0; targetIndex != work.effect.count; targetIndex++) {
    struct AnimationObject *animation =
        BattleObjGet(work.effect.actors[targetIndex])->object->records;
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetIndex][entryIndex] =
          animation->entries[entryIndex]->param;
  }
  /* FAKEMATCH(one-pass): the five effects in a one-pass block, which places
   * each call's argument before its kind's store; as plain statements, 43 of
   * 180 instructions differ in tbs-en. */
  do {
    work.effect.kind = ELEMENT_VENUS;
    work.effect.variant = 0;
    BattleEffect_RunElementPillarFar(&work.effect);
    work.effect.kind = ELEMENT_MERCURY;
    BattleEffect_RunElementPillarFar(&work.effect);
    work.effect.kind = ELEMENT_MARS;
    BattleEffect_RunElementPillarFar(&work.effect);
    work.effect.kind = ELEMENT_JUPITER;
    BattleEffect_RunElementPillarFar(&work.effect);
    work.effect.kind = 0;
    BattleEffect_RunTableEffectFar(&work.effect);
    ObjectDispatch_SelectAnimationFar(object, POSE_STANDING);
  } while (0);
  for (targetIndex = 0; targetIndex != work.effect.count; targetIndex++)
    ObjMotRst(work.effect.actors[targetIndex]);
  ObjMotRst(work.effect.actor);
  return 0;
}

/// Auto battle: deals damage HP to unit unitId and shows it. When showMessage
/// is set, "A bitter blow!" (to the party) or "A critical hit!" (to an enemy)
/// comes first; then `<unit> takes <damage> damage!`, and `<unit> goes down...`
/// or `<unit> has been downed!` when the HP runs out.
/// context, if null, is replaced by four cleared bytes nothing reads.
/// Nothing in the source calls it. Lowers unit->hp, no lower than 0.
void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 showMessage,
                                        u8 *context) {
  u8 localContext[4];
  struct BattleUnit *unit;
  struct BattleObj *battleObject;

  if (context == 0) {
    context = localContext;
    context[0] = 0;
    context[1] = 0;
    context[2] = 0;
    context[3] = 0;
  }

  unit = UnitGetFar(unitId);
  unit->hp -= damage;
  if (unit->hp < 0)
    unit->hp = 0;

  // The damage pose, and the status window with names and HP only.
  battleObject = BattleObjGet(unitId);
  ObjectDispatch_SelectAnimationFar(battleObject->object, POSE_DAMAGE);
  PartyStatusDrawFar(PARTY_STATUS_HP_ONLY);
  UiWork_ClearValueNameTablesFar();

  if (unitId <= ALLY_MAX) {
    if (showMessage != 0)
      UiText_ShowMessageAndWaitCoreFar(MsgBitterBlow);
    UiWork_PushValueSlotFar(damage, VALUE_NUMBER);
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgDmgP);
  } else {
    if (showMessage != 0)
      UiText_ShowMessageAndWaitCoreFar(MsgCritical);
    UiWork_PushValueSlotFar(damage, VALUE_NUMBER);
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgDmgE);
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
  }

  BattleMotion_SetMode5AndActivateSlot(unitId);
  if (unitId <= ALLY_MAX) {
    if (unit->hp <= 0) {
      UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgGoesDown);
    }
  } else if (unit->hp <= 0) {
    UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgDowned);
  }

  battleObject = BattleObjGet(unitId);
  ObjectDispatch_SelectAnimationFar(battleObject->object, POSE_STANDING);
}

/// For a unit whose HP has run out: plays its falling animation to the last
/// frame and marks its object for release (BattleObject_MarkForRelease).
/// Called by BattlePresentation_ApplyUnitDamage.
void BattleMotion_SetMode5AndActivateSlot(s32 unitId) {
  struct AnimationEntry *actorData;
  struct AnimationObject *object;
  if (UnitGetFar(unitId)->hp <= 0) {
    // POSE_DAMAGE in grey, its first entry set to frame 0xff, past its last.
    object = BattleObjGet(unitId)->object->records;
    AnimationObjects_SelectAnimationFar(object, POSE_DAMAGE);
    actorData = object->entries[0];
    actorData->param = SPRITE_COLOR_GREY;
    actorData->frame = 0xFF;
    WaitFrames(4);
    Map_RenderAnimatedTileFramesForObjectFar((u8 *)object);
    BattleObject_MarkForRelease(unitId);
  }
}

/// BattleTarget_ReplaceDefeated's result when the target's side is all down.
/// It does not come up: Battle_RunEncounter ends the battle as soon as a
/// side is down, before the next action.
#define TARGET_SIDE_DEFEATED 0x100

/// Returns the action's target, or a random living unit of its side when
/// the target is down; TARGET_SIDE_DEFEATED when none is left. Called by
/// BattlePresentation_RunAction.
s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action) {
  u16 livingUnits[14];
  s32 targetId;
  s32 livingCount;

  targetId = action->target;
  if (UnitGetFar(targetId)->hp != 0) {
    return targetId;
  }

  if (targetId > FIRST_ENEMY_UNIT - 1) {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
  } else {
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
  }

  if (livingCount == 0) {
    return TARGET_SIDE_DEFEATED;
  }

  return (s16)livingUnits[(Random16() * livingCount) >> 0x10];
}
