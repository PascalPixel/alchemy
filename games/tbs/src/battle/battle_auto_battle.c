/// @file
/// Auto battle: the actions it builds and plays, without the command menu.
// Part of battle.c.

#include "battle_internal.h"

/// Auto battle: gives every living unit a plain attack and sorts them into
/// action order. Returns how many actions there are.
s32 AutoBattle_BuildActions(struct BattleAction *entries) {
  struct BattleAction swap;
  u16 unitIds[UNIT_MAX];
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
    entry->priority = unit->agility;
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

    unit = Unit_GetFar(unitId);
    entry->unitId = unitId;
    entry->priority = unit->agility >> 1;
    if (entry->priority != 0)
      entry->priority += Random16_Below(unit->agility);
    entry->command = COMMAND_ATTACK;
    entry->parameter = 0;
    entry->target = Random16_Below(targetRange);
    count++;
    entry++;
  }

  // Bubble the faster units forward.
  // BUG: one pass short of a full sort; with two actions nothing is sorted.
  for (index = count - 2; index > 0; index--) {
    s32 swaps = 0;
    s32 pos;

    for (pos = count - 1; pos > 0; pos--) {
      if (entries[pos].priority > entries[pos - 1].priority) {
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

/// Unused: turns the camera while L or R is held.
DEFINE_ADJUST_CAMERA(BattleCamera_AdjustByShoulderKeys)

/// Auto battle: plays one action, a defeated target replaced by a living unit
/// of its side. Returns 1 when the party got away, -1 when the unit is down.
s32 AutoBattle_RunAction(struct BattleAction *action) {
  struct BattleBackgroundState *view;
  s32 unitId;
  s32 yaw;
  struct Unit *actor;

  unitId = action->unitId;
  actor = Unit_GetFar(unitId);
  if (actor->hp == 0)
    return -1;

  action->target = BattleAbility_ReplaceDefeated(action);
  view = gBattleBackgroundState;
  // Here any unit id above Felix's counts as an enemy, Jenna's included.
  if (action->unitId > FELIX)
    yaw = YAW_ENEMY_ACTION;
  else
    yaw = YAW_PARTY_ACTION;
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
    wait(45);
    AutoBattle_ShowDefend(action);
    break;
  case COMMAND_ITEM:
    wait(45);
    AutoBattle_RunAttackOrItem(action);
    break;
  case COMMAND_ATTACK:
  default: {
    struct BattleBackgroundState *view = gBattleBackgroundState;
    view->keepRange = 0;
    AutoBattle_RunAttackOrItem(action);
    view->keepRange = 0;
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
s32 BattleFlee_PlayRun(struct BattleAction *action) {
  s16 partyMembers[UNIT_MAX];
  s32 partySize;
  s32 memberSlot;

  (void)action;
  // Flee unless a roll below 16 comes up 0, 15 in 16.
  if (Random16_Below(16) != 0) {
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
s32 AutoBattle_ShowDefend(const struct BattleAction *action) {
  s32 unitId;
  s32 result = 0;
  struct Unit *unit;

  unitId = action->unitId;
  unit = Unit_GetFar(unitId);
  if (BattleObject_IsValidId(unitId) < 0) {
    return -1;
  }
  if (unit->hp <= 0) {
    return result;
  }
  MessageArgument_ClearAllFar();
  MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar(MsgActorDefends);
  return 0;
}

/// Auto battle: casts Psynergy, the caster's side fading out while its effect
/// plays on the target side.
s32 AutoBattle_RunPsynergy(struct BattleAction *action) {
  u16 visibleUnits[UNIT_MAX];
  struct BattleStagingState context;
  s32 unitId;
  s32 targetId;
  u32 sideStart;
  u32 sideCount;
  u32 livingCount;
  struct Actor *casterActor;
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
    struct BattleBackgroundState *view = gBattleBackgroundState;
    // An enemy caster turns the camera to YAW_BEHIND_ENEMIES, not
    // YAW_ENEMY_ACTION.
    view->targetYaw =
        action->unitId > FELIX ? YAW_BEHIND_ENEMIES : YAW_PARTY_ACTION;
    view->frames = 60;
  }
  wait(10);
  // A roll nothing uses, which still advances the random number generator.
  Random16();
  casterActor = BattleObject_Get(unitId)->actor;

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
      Actor_SelectAnimationFar(casterActor, POSE_CAST);
    }
  }

  // Fade the caster's side out over 16 frames, and close the party status
  // window.
  wait(30);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  }
  for (index = 0; index != BLEND_WEIGHT_MAX; index++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - index, BLEND_WEIGHT_MAX);
    wait(1);
  }
  PartyStatus_DrawFar(PARTY_STATUS_CLOSED);

  // Ready only the target side's living units, and play the effect.
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
    context.effect.side = EFFECT_SIDE_PARTY;
  } else {
    context.effect.side = EFFECT_SIDE_ENEMIES;
  }
  BattleEffect_RunSummonFar(&context.effect);

  // Bring every unit back and fade the caster's side in.
  wait(10);
  BattleObject_Put();
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (index = 0; index != sideCount; index++) {
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
  }
  for (index = 0; index != BLEND_WEIGHT_MAX; index++) {
    REG_BLDALPHA = BLEND_ALPHA(index, BLEND_WEIGHT_MAX);
    wait(1);
  }
  for (index = 0; index != sideCount; index++) {
    BattleHighlight_SetSpriteMode(index + sideStart, SPRITE_OPAQUE);
  }
  BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
  wait(3);
  return 0;
}

/// Unused auto battle attack: "<unit> attacks!", the attacker runs up to its
/// target, the blow, and both return.
s32 AutoBattle_RunApproachAttack(struct BattleAction *input) {
  struct BattleStagingState state;

  // A short wait when the camera already faces the party's side.
  if (gBattleBackgroundState->targetYaw == YAW_PARTY_ACTION) {
    gBattleBackgroundState->targetYaw = YAW_PARTY_ACTION;
    wait(10);
  } else {
    gBattleBackgroundState->targetYaw = YAW_PARTY_ACTION;
    wait(30);
  }

  state.effect.user = input->unitId;
  if (BattleObject_IsValidId(state.effect.user) < 0)
    return -1;

  state.effect.targets[0] = input->target;
  if (BattleObject_IsValidId(state.effect.targets[0]) < 0)
    return -1;

  // Lookups whose results are unused, and a roll that only advances the
  // random number generator.
  Unit_GetFar(state.effect.user);
  Unit_GetFar(state.effect.targets[0]);
  Random16();
  MessageArgument_PushFar(state.effect.user, MESSAGE_ARGUMENT_ACTOR);
  BattleMessage_ShowAndWaitFar(MsgActorAttacks);
  BattleMotion_ApproachTarget(state.effect.user, state.effect.targets[0], 13,
                              0);
  Actor_SetAnimationSpeedFar(BattleObject_Get(state.effect.user)->actor,
                             ANIM_STEP_NORMAL);
  BattleObject_Get(state.effect.targets[0]);

  state.effect.targetCount = 1;
  if ((u16)state.effect.targets[0] <= ALLY_MAX)
    state.effect.side = EFFECT_SIDE_PARTY;
  else
    state.effect.side = EFFECT_SIDE_ENEMIES;
  state.effect.prelude = 0;

  wait(4);
  BattleEffect_RunByIdRangeFar(&state.effect);
  BattleMotion_ReturnToSpot(state.effect.targets[0]);
  BattleMotion_ReturnToSpot(state.effect.user);
  return 0;
}

/// Auto battle: an attack or an item, played on the first living unit of the
/// target's side.
s32 AutoBattle_RunAttackOrItem(struct BattleAction *action) {
  struct BattleStagingState state;
  struct BattleBackgroundState *view = gBattleBackgroundState;
  struct Actor *userActor;
  s32 targetYaw;
  s32 targetIndex;

  // As in AutoBattle_RunAction, Jenna counts as an enemy here.
  if (action->unitId > FELIX)
    targetYaw = YAW_ENEMY_ACTION;
  else
    targetYaw = YAW_PARTY_ACTION;
  if (view->targetYaw == targetYaw) {
    view->frames = 40;
    wait(40);
  } else {
    view->targetYaw = targetYaw;
    view->frames = 40;
    wait(40);
  }
  state.effect.kind = action->parameter;
  state.effect.range = action->range;
  state.effect.user = action->unitId;
  state.effect.firstTarget = action->target;
  if (BattleObject_IsValidId(state.effect.user) < 0)
    return -1;
  if (state.effect.firstTarget > FIRST_ENEMY_UNIT - 1)
    state.effect.targetCount =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, state.effect.targets);
  else
    state.effect.targetCount =
        BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, state.effect.targets);
  // The status window without its PP row (see PARTY_STATUS_SUMMONS).
  PartyStatus_DrawFar(gBattleState->partyStatus & ~PARTY_STATUS_SHOW_PP);
  // The unit raises its hand (POSE_CAST); then only the first living unit of
  // the target's side takes the effect.
  userActor = BattleObject_Get(state.effect.user)->actor;
  Actor_SelectAnimationFar(userActor, POSE_CAST);
  Actor_SetAnimationSpeedFar(userActor, ANIM_STEP_NORMAL);
  if ((u16)action->target <= ALLY_MAX) {
    state.effect.side = EFFECT_SIDE_PARTY;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, state.effect.targets);
    state.effect.targetCount = 1;
  } else {
    state.effect.side = EFFECT_SIDE_ENEMIES;
    BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, state.effect.targets);
    state.effect.targetCount = 1;
  }

  // Keep each target's sprite entries' parameters for the effect.
  for (targetIndex = 0; targetIndex != state.effect.targetCount;
       targetIndex++) {
    struct Sprite *sprite =
        BattleObject_Get(state.effect.targets[targetIndex])->actor->sprite;
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != sprite->count - 1; entryIndex++)
      state.savedAnimationParameters[targetIndex][entryIndex] =
          sprite->entries[entryIndex]->colorMode;
  }
  // The four elements' pillars in turn, then the blow.
  state.effect.kind = ELEMENT_VENUS;
  state.effect.variant = 0;
  BattleEffect_RunElementPillarFar(&state.effect);
  state.effect.kind = ELEMENT_MERCURY;
  BattleEffect_RunElementPillarFar(&state.effect);
  state.effect.kind = ELEMENT_MARS;
  BattleEffect_RunElementPillarFar(&state.effect);
  state.effect.kind = ELEMENT_JUPITER;
  BattleEffect_RunElementPillarFar(&state.effect);
  state.effect.kind = 0;
  /* FAKEMATCH(dead): a never-true test, kind being 0, and its arm; as
   * BattleEffect_RunFromTableFar(&state.effect) alone. */
  if (state.effect.kind)
    BattleEffect_RunByIdRangeFar(&state.effect);
  else
    BattleEffect_RunFromTableFar(&state.effect);
  Actor_SelectAnimationFar(userActor, POSE_STANDING);
  for (targetIndex = 0; targetIndex != state.effect.targetCount; targetIndex++)
    BattleMotion_ReturnToSpot(state.effect.targets[targetIndex]);
  BattleMotion_ReturnToSpot(state.effect.user);
  return 0;
}

/// Unused auto battle damage: takes damage HP from a unit and shows it, with "A
/// bitter blow!" or "A critical hit!" first when showMessage is set.
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

/// For a unit whose HP has run out: plays its falling effect to the last frame
/// and marks its battle object for release (BattleObject_MarkForRelease).
void BattleMotion_PlayFall(s32 unitId) {
  struct SpriteAnimation *entry;
  struct Sprite *sprite;
  if (Unit_GetFar(unitId)->hp <= 0) {
    // POSE_DAMAGE in grey, its first entry showing SPRITE_PICTURE_NONE.
    sprite = BattleObject_Get(unitId)->actor->sprite;
    Sprite_StartAnimationFar(sprite, POSE_DAMAGE);
    entry = sprite->entries[0];
    entry->colorMode = SPRITE_COLOR_GREY;
    entry->picture = SPRITE_PICTURE_NONE;
    wait(4);
    SpriteDissolve_RunFar((u8 *)sprite);
    BattleObject_MarkForRelease(unitId);
  }
}

/// BattleAbility_ReplaceDefeated's result when the target's side is all down,
/// which the battle's end comes before.
#define TARGET_SIDE_DEFEATED 256

/// Returns the action's target, or a random living unit of its side when the
/// target is down; TARGET_SIDE_DEFEATED when none is left.
s32 BattleAbility_ReplaceDefeated(const struct BattleAction *action) {
  u16 livingUnits[UNIT_MAX];
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

  return (s16)livingUnits[Random16_Below(livingCount)];
}
