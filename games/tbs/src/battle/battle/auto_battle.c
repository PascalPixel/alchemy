/// @file
/// Auto battle: the actions it builds and plays, without the command menu.
// Part of battle.c.

#include "../battle_internal.h"

/// Auto battle: gives every living unit a plain attack and sorts them into
/// turn order. The party act at their Agility and target the first enemy
/// unit, 128; enemies act at half their Agility plus a random part of it and
/// target unit id 0 to n - 1 for n living party members, which
/// AutoBattle_RunAction replaces if it is down. Returns how many
/// actions there are. Called by Battle_Run each round under
/// FLAG_AUTO_BATTLE; fills entries.
s32 AutoBattle_BuildActions(struct BattleAction *entries) {
  struct BattleAction swap;
  u16 unitIds[14];
  s32 partyCount;
  s32 count = 0;
  s32 enemyCount;
  s32 targetRange;
  s32 index;
  struct BattleAction *entry;
  struct Unit *unit;

  // The party's actions. The four lookups' results are never used.
  partyCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
  for (index = 0; index != PARTY_BATTLE_SIZE; index++)
    Unit_GetFar(index);
  for (index = 0; index < partyCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_GetFar(unitId);
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
  // AutoBattle_RunAction replaces if the unit is down.
  enemyCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
  entry = &entries[partyCount];
  targetRange = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, NULL);
  for (index = 0; index < enemyCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_GetFar(unitId);
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
DEFINE_ADJUST_CAMERA(BattleCamera_AdjustByShoulderKeys)

/// Auto battle: plays one action. A unit already down does nothing (-1);
/// a defeated target is replaced by a living unit of its side first.
/// Returns 1 when the party got away, 0 otherwise. Called by
/// Battle_Run for each action under FLAG_AUTO_BATTLE. Turns the
/// camera to the acting unit's side (gBattleBackgroundWork) and rewrites
/// action->target.
s32 AutoBattle_RunAction(struct BattleAction *action) {
  struct BattleBackgroundWork *view;
  s32 unitId;
  s32 yaw;
  struct Unit *actor;

  unitId = action->unitId;
  actor = Unit_GetFar(unitId);
  if (actor->hp == 0)
    return -1;

  action->target = BattleAbility_ReplaceDefeated(action);
  view = gBattleBackgroundWork;
  // Here any unit id above 4 counts as an enemy, Jenna's 5 included
  // (character_ids.h).
  if (action->unitId > 4)
    yaw = YAW_ENEMY_TURN;
  else
    yaw = YAW_PARTY_TURN;
  view->targetYaw = yaw;
  view->frames = 60;
  MessageArgument_ClearAllFar();

  switch (action->command) {
  case COMMAND_FLEE:
    BattleMessage_ShowAndWaitFar(MsgPartyFlees);
    if (BattleFlee_PlayRun(action) != 0)
      return 1;
    break;
  case COMMAND_DEFEND:
    WaitFrames(45);
    AutoBattle_ShowDefend(action);
    break;
  case COMMAND_ITEM:
    WaitFrames(45);
    AutoBattle_RunAttackOrItem(action);
    break;
  case COMMAND_ATTACK:
  default: {
    struct BattleBackgroundWork *view = gBattleBackgroundWork;
    view->flag = 0;
    AutoBattle_RunAttackOrItem(action);
    view->flag = 0;
    break;
  }
  case COMMAND_PSYNERGY:
    AutoBattle_RunPsynergy(action);
    break;
  }

  TextPrinter_ResetFreeFar();
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
/// living party member running off in turn. Returns 1 when they got away; else
/// shows MsgCannotEscape and returns 0. Called by AutoBattle_RunAction.
s32 BattleFlee_PlayRun(struct BattleAction *action) {
  s16 partyMembers[14];
  s32 partySize;
  s32 animated;
  s32 memberSlot;

  (void)action;
  // The roll's top four bits: flee unless all are clear, 15 in 16.
  if (((Random16() << 4) >> 16) != 0) {
    partySize = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, partyMembers);
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
  BattleMessage_ShowAndWaitFar(MsgCannotEscape);
  return 0;
}

/// Auto battle: shows that a standing unit is defending. The message is
/// `<unit> is defending`. Returns -1 for an invalid unit id, 0 otherwise.
/// Called by AutoBattle_RunAction.
s32 AutoBattle_ShowDefend(const struct BattleAction *action) {
  s32 unitId;
  s32 result;
  struct Unit *unit;

  unitId = action->unitId;
  unit = Unit_GetFar(unitId);
  if (BattleObject_IsValidId(unitId) < 0) {
    return -1;
  }
  result = 0;
  if (unit->hp <= 0) {
    return result;
  }
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar(MsgActorDefends);
  return 0;
}

/// Auto battle: casts Psynergy. The camera turns to the caster's side and
/// the caster raises its hand; the target side fades out, its living units
/// are readied alone, the Psynergy's animation (kind action->parameter) plays
/// on them, and everyone fades back in. Returns -1 for an invalid unit or
/// target. Called by AutoBattle_RunAction for COMMAND_PSYNERGY. Writes
/// REG_BLDCNT and REG_BLDALPHA and the actors' colour modes, and leaves the
/// battle objects readied again.
s32 AutoBattle_RunPsynergy(struct BattleAction *action) {
  u16 visibleUnits[14];
  struct BattleStagingWork context;
  s32 unitId;
  s32 targetId;
  u32 sideStart;
  u32 sideCount;
  u32 livingCount;
  struct Actor *actorRecord;
  u32 index;

  livingCount = 0;
  unitId = action->unitId;
  if (BattleObject_IsValidId(unitId) < 0) {
    return -1;
  }
  targetId = action->target;
  if (BattleObject_IsValidId(targetId) < 0) {
    return -1;
  }

  {
    struct BattleBackgroundWork *view = gBattleBackgroundWork;
    // An enemy caster turns the camera to YAW_BEHIND_ENEMIES, not
    // YAW_ENEMY_TURN.
    view->targetYaw = action->unitId > 4 ? YAW_BEHIND_ENEMIES : YAW_PARTY_TURN;
    view->frames = 60;
  }
  WaitFrames(10);
  // A roll nothing uses, which still advances the random number generator.
  Random16();
  actorRecord = BattleObject_Get(unitId)->actor;

  // The caster's side, taken as the side the target is not on, and the
  // caster's POSE_CAST when its id is among that side's first sideCount ids.
  if (targetId >= 0 && targetId <= ALLY_MAX) {
    sideCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
    sideStart = FIRST_ENEMY_UNIT;
  } else {
    sideCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
    sideStart = 0;
  }

  for (index = 0; index != sideCount; index++) {
    if (index + sideStart == unitId) {
      Actor_SelectAnimationFar(actorRecord, POSE_CAST);
    }
  }

  // Fade the caster's side out: its sprites SPRITE_SEMI_TRANSPARENT,
  // their blend weight falling from 16 to 1 over 16 frames. Then the party
  // status window is erased (PARTY_STATUS_CLOSED).
  WaitFrames(30);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  }
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = BLEND_ALPHA(16 - index, 16);
    WaitFrames(1);
  }
  PartyStatus_DrawFar(PARTY_STATUS_CLOSED);

  // Ready only the target side's living units, and play the effect. The
  // walk counts unit ids up from the side's first until it has found as
  // many living units as the side has.
  if (targetId > FIRST_ENEMY_UNIT - 1) {
    u32 count = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      u32 unit = index + FIRST_ENEMY_UNIT;
      if (Unit_GetFar(unit)->hp > 0) {
        visibleUnits[livingCount++] = unit;
      }
    }
  } else {
    u32 count = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
    for (index = 0; livingCount != count; index++) {
      if (Unit_GetFar(index)->hp > 0) {
        visibleUnits[livingCount++] = index;
      }
    }
  }
  visibleUnits[livingCount] = BATTLE_UNIT_LIST_END;
  BattleObject_SpawnObjectsForList(visibleUnits, 0);

  context.effect.kind = action->parameter;
  context.effect.user = unitId;
  for (index = 0; index != livingCount; index++) {
    context.effect.targets[index] = visibleUnits[index];
  }
  context.effect.targetCount = livingCount;
  if (targetId >= 0 && targetId <= ALLY_MAX) {
    context.effect.side = ANIM_SIDE_PARTY;
  } else {
    context.effect.side = ANIM_SIDE_ENEMIES;
  }
  BattleAnim_RunSummonFar(&context.effect);

  // Bring every unit back and fade the caster's side in.
  WaitFrames(10);
  BattleObject_Put();
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  }
  for (index = 0; index != 16; index++) {
    REG_BLDALPHA = BLEND_ALPHA(index, 16);
    WaitFrames(1);
  }
  for (index = 0; index != sideCount; index++) {
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_OPAQUE);
  }
  BattleCamera_SetView(0, 0, 0, 0x64);
  WaitFrames(3);
  return 0;
}

/// Auto battle: an attack. Shows `<unit> attacks!`, sends the attacker up
/// to its target, plays the blow and sends both back. Returns -1 for an
/// invalid attacker or target. Nothing in the source calls it.
s32 AutoBattle_RunApproachAttack(struct BattleAction *input) {
  struct BattleStagingWork work;

  // A short wait when the camera already faces the party's turn.
  if (gBattleBackgroundWork->targetYaw == YAW_PARTY_TURN) {
    gBattleBackgroundWork->targetYaw = YAW_PARTY_TURN;
    WaitFrames(10);
  } else {
    gBattleBackgroundWork->targetYaw = YAW_PARTY_TURN;
    WaitFrames(30);
  }

  work.effect.user = input->unitId;
  if (BattleObject_IsValidId(work.effect.user) < 0)
    return -1;

  work.effect.targets[0] = input->target;
  if (BattleObject_IsValidId(work.effect.targets[0]) < 0)
    return -1;

  // Lookups whose results are unused, and a roll that only advances the
  // random number generator.
  Unit_GetFar(work.effect.user);
  Unit_GetFar(work.effect.targets[0]);
  Random16();
  MessageArgument_PushFar(work.effect.user, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar(MsgActorAttacks);
  BattleMotion_ApproachTarget(work.effect.user, work.effect.targets[0], 13, 0);
  Actor_SetAnimationSpeedFar(BattleObject_Get(work.effect.user)->actor, 16);
  BattleObject_Get(work.effect.targets[0]);

  work.effect.targetCount = 1;
  if ((u16)work.effect.targets[0] <= ALLY_MAX)
    work.effect.side = ANIM_SIDE_PARTY;
  else
    work.effect.side = ANIM_SIDE_ENEMIES;
  work.effect.prelude = 0;

  WaitFrames(4);
  BattleAnim_RunByIdRangeFar(&work.effect);
  BattleMotion_ReturnToSpot(work.effect.targets[0]);
  BattleMotion_ReturnToSpot(work.effect.user);
  return 0;
}

/// Auto battle: an attack or an item. Turns the camera to the acting unit's
/// side, poses the unit and plays the animation (kind action->parameter) on the
/// first living unit of the target's side, then sends both back. Returns -1
/// for an invalid unit. Called by AutoBattle_RunAction for
/// COMMAND_ATTACK, COMMAND_ITEM and any other command but fleeing, defending
/// and Psynergy.
s32 AutoBattle_RunAttackOrItem(struct BattleAction *action) {
  struct BattleStagingWork work;
  struct BattleBackgroundWork *view = gBattleBackgroundWork;
  struct Actor *object;
  s32 targetYaw;
  s32 targetIndex;

  if (action->unitId > 4)
    targetYaw = YAW_ENEMY_TURN;
  else
    targetYaw = YAW_PARTY_TURN;
  if (view->targetYaw == targetYaw) {
    view->frames = 40;
    WaitFrames(40);
  } else {
    view->targetYaw = targetYaw;
    view->frames = 40;
    WaitFrames(40);
  }
  work.effect.kind = action->parameter;
  work.effect.range = action->range;
  work.effect.user = action->unitId;
  work.effect.firstTarget = action->target;
  if (BattleObject_IsValidId(work.effect.user) < 0)
    return -1;
  if (work.effect.firstTarget > FIRST_ENEMY_UNIT - 1)
    work.effect.targetCount =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, work.effect.targets);
  else
    work.effect.targetCount =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, work.effect.targets);
  // The status window without its PP row (see PARTY_STATUS_SUMMONS).
  PartyStatus_DrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);
  // The unit raises its hand (POSE_CAST); then only the first living unit of
  // the target's side takes the effect.
  object = BattleObject_Get(work.effect.user)->actor;
  Actor_SelectAnimationFar(object, POSE_CAST);
  Actor_SetAnimationSpeedFar(object, 16);
  if ((u16)action->target <= ALLY_MAX) {
    work.effect.side = ANIM_SIDE_PARTY;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, work.effect.targets);
    work.effect.targetCount = 1;
  } else {
    work.effect.side = ANIM_SIDE_ENEMIES;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, work.effect.targets);
    work.effect.targetCount = 1;
  }

  // Keep each target's sprite entries' parameters for the effect.
  for (targetIndex = 0; targetIndex != work.effect.targetCount; targetIndex++) {
    struct Sprite *animation =
        BattleObject_Get(work.effect.targets[targetIndex])->actor->sprite;
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetIndex][entryIndex] =
          animation->entries[entryIndex]->colorMode;
  }
  /* FAKEMATCH(one-pass): the five effects in a one-pass block, which places
   * each call's argument before its kind's store; as plain statements, 43 of
   * 180 instructions differ in tbs-en. */
  do {
    work.effect.kind = ELEMENT_VENUS;
    work.effect.variant = 0;
    BattleAnim_RunElementPillarFar(&work.effect);
    work.effect.kind = ELEMENT_MERCURY;
    BattleAnim_RunElementPillarFar(&work.effect);
    work.effect.kind = ELEMENT_MARS;
    BattleAnim_RunElementPillarFar(&work.effect);
    work.effect.kind = ELEMENT_JUPITER;
    BattleAnim_RunElementPillarFar(&work.effect);
    work.effect.kind = 0;
    BattleAnim_RunFromTableFar(&work.effect);
    Actor_SelectAnimationFar(object, POSE_STANDING);
  } while (0);
  for (targetIndex = 0; targetIndex != work.effect.targetCount; targetIndex++)
    BattleMotion_ReturnToSpot(work.effect.targets[targetIndex]);
  BattleMotion_ReturnToSpot(work.effect.user);
  return 0;
}

/// Auto battle: deals damage HP to unit unitId and shows it. When showMessage
/// is set, "A bitter blow!" (to the party) or "A critical hit!" (to an enemy)
/// comes first; then `<unit> takes <damage> damage!`, and `<unit> goes down...`
/// or `<unit> has been downed!` when the HP runs out.
/// context, if null, is replaced by four cleared bytes nothing reads.
/// Nothing in the source calls it. Lowers unit->hp, no lower than 0.
void AutoBattle_ApplyDamage(u32 unitId, s32 damage, s32 showMessage,
                            u8 *context) {
  u8 localContext[4];
  struct Unit *unit;
  struct BattleObject *battleObject;

  if (context == 0) {
    context = localContext;
    context[0] = 0;
    context[1] = 0;
    context[2] = 0;
    context[3] = 0;
  }

  unit = Unit_GetFar(unitId);
  unit->hp -= damage;
  if (unit->hp < 0)
    unit->hp = 0;

  // The damage pose, and the status window with names and HP only.
  battleObject = BattleObject_Get(unitId);
  Actor_SelectAnimationFar(battleObject->actor, POSE_DAMAGE);
  PartyStatus_DrawFar(PARTY_STATUS_HP_ONLY);
  MessageArgument_ClearAllFar();

  if (unitId <= ALLY_MAX) {
    if (showMessage != 0)
      BattleMessage_ShowAndWaitFar(MsgBitterBlow);
    MessageArgument_PushFar(damage, MESSAGE_ARGUMENT_NUMBER);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgDmgP);
  } else {
    if (showMessage != 0)
      BattleMessage_ShowAndWaitFar(MsgCritical);
    MessageArgument_PushFar(damage, MESSAGE_ARGUMENT_NUMBER);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgDmgE);
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  }

  BattleMotion_PlayFall(unitId);
  if (unitId <= ALLY_MAX) {
    if (unit->hp <= 0) {
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar(MsgGoesDown);
    }
  } else if (unit->hp <= 0) {
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgDowned);
  }

  battleObject = BattleObject_Get(unitId);
  Actor_SelectAnimationFar(battleObject->actor, POSE_STANDING);
}

/// For a unit whose HP has run out: plays its falling animation to the last
/// frame and marks its battle object for release (BattleObject_MarkForRelease).
/// Called by AutoBattle_ApplyDamage.
void BattleMotion_PlayFall(s32 unitId) {
  struct SpriteAnim *actorData;
  struct Sprite *animation;
  if (Unit_GetFar(unitId)->hp <= 0) {
    // POSE_DAMAGE in grey, its first entry set to frame 0xff, past its last.
    animation = BattleObject_Get(unitId)->actor->sprite;
    Sprite_StartAnimFar(animation, POSE_DAMAGE);
    actorData = animation->entries[0];
    actorData->colorMode = SPRITE_COLOR_GREY;
    actorData->picture = 0xFF;
    WaitFrames(4);
    SpriteDissolve_RunFar((u8 *)animation);
    BattleObject_MarkForRelease(unitId);
  }
}

/// BattleAbility_ReplaceDefeated's result when the target's side is all down.
/// It does not come up: Battle_Run ends the battle as soon as a
/// side is down, before the next action.
#define TARGET_SIDE_DEFEATED 0x100

/// Returns the action's target, or a random living unit of its side when
/// the target is down; TARGET_SIDE_DEFEATED when none is left. Called by
/// AutoBattle_RunAction.
s32 BattleAbility_ReplaceDefeated(const struct BattleAction *action) {
  u16 livingUnits[14];
  s32 targetId;
  s32 livingCount;

  targetId = action->target;
  if (Unit_GetFar(targetId)->hp != 0) {
    return targetId;
  }

  if (targetId > FIRST_ENEMY_UNIT - 1) {
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
  } else {
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
  }

  if (livingCount == 0) {
    return TARGET_SIDE_DEFEATED;
  }

  return (s16)livingUnits[(Random16() * livingCount) >> 0x10];
}
