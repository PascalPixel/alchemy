/// @file
/// Playing one action of a turn: the staging its ability calls for (an attack,
/// a cast, a summon over a faded field, a flight), and the extra strikes that
/// Daedalus, Megaera and Iris add.
// Part of battle.c.

#include "battle_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "battle/battle.h"
#include "battle_effect/battle_effect_ids.h"
#include "gamepak/far_call/battle_effect.h"
#include "battle/presentation.h"

/// A party member's attack effect, staged by id range (EFFECT_BY_ID_RANGE):
/// ATTACK_EFFECT_SLASH for Isaac, Garet, Felix, Jenna and Piers,
/// ATTACK_EFFECT_WAVE for Ivan, Mia and Sheba, as their gBattleSprites rows
/// give, any other none.
s32 BattleUnit_GetPartyAttackEffect(s32 characterId) {
  s32 effect;

  /* FAKEMATCH(dead): effect set to characterId, a store the next line
   * overwrites. */
  effect = characterId;
  effect = 0;
  switch (characterId) {
  case ISAAC:
  case GARET:
  case FELIX:
  case JENNA:
  case PIERS:
    effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_SLASH;
    break;
  case IVAN:
  case MIA:
  case SHEBA:
    effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_WAVE;
    break;
  }
  return effect;
}

/// Called by Battle_Run at the start of every turn; does nothing.
void Battle_TurnStartNoOp(void) {}

/// The plan of the extra action Daedalus queues for the turn's end
/// (COMMAND_DAEDALUS): ABILITY_DAEDALUS_STRIKE from a random unit of the
/// queuing side against every unit of the other, staged as the summon
/// SUMMON_EFFECT_DAEDALUS_STRIKE. source is the queued BattleActionRecord, read
/// as halfwords: [0] its unit, [3] its command. Declared s32, though it returns
/// no value; its one caller reads none.
s32 BattleStaging_PlanDaedalus(u16 *source, struct BattlePlan *plan) {
  u16 ids[10];
  s32 count;
  s32 i;
  s32 side = BATTLE_SIDE_MASK_ENEMIES;
  s32 own;

  if (source[0] > ALLY_MAX)
    side = BATTLE_SIDE_MASK_PARTY;
  count = BattleUnit_ListLivingIds(side, ids);
  // The plan and BattleState.skipPlayback after it.
  Iwram_ClearWordsEntry(plan, sizeof(*plan) + sizeof(s32));
  own = 0;
  if (source[0] <= ALLY_MAX)
    own = 1;
  own = Battle_GetUnitAtPlace(BattleAI_ChooseRandomTarget(own));
  plan->element = ELEMENT_MARS;
  plan->staging = STAGING_DAEDALUS;
  {
    s32 command = source[3];

    plan->unitId = own;
    plan->command = command;
  }
  for (i = 0; i < count; i++) {
    plan->targetIds[i] = ids[i];
    plan->targetOffset[i] = 0;
    plan->targetMultiplier[i] = 1;
    plan->targetCritical[i] = 0;
    plan->targetHit[i] = 1;
  }
  plan->targetCount = count;
  plan->abilityId = ABILITY_DAEDALUS_STRIKE;
  plan->battleEffect = EFFECT_SUMMON | SUMMON_EFFECT_DAEDALUS_STRIKE;
}

/// The plan of Megaera's second part (STAGING_MEGAERA): High Impact
/// (ABILITY_HIGH_IMPACT) from the acting unit on every living unit of its own
/// side.
void BattleStaging_PlanHighImpact(struct BattlePlan *plan) {
  u16 ids[10];
  s32 count;
  s32 i;
  s32 side = BATTLE_SIDE_MASK_PARTY;

  if (plan->unitId > ALLY_MAX)
    side = BATTLE_SIDE_MASK_ENEMIES;
  count = BattleUnit_ListLivingIds(side, ids);
  for (i = 0; i < count; i++) {
    plan->targetIds[i] = ids[i];
    plan->targetOffset[i] = 0;
    plan->targetMultiplier[i] = 1;
    plan->targetCritical[i] = 0;
    plan->targetHit[i] = 1;
  }
  plan->targetCount = count;
  plan->abilityId = ABILITY_HIGH_IMPACT;
  plan->battleEffect = 1 << EFFECT_VARIANT_SHIFT | BATTLE_EFFECT_CRUCIBLE_FIRE;
}

/// The plan of Iris's second part (STAGING_IRIS): Aurora Field
/// (ABILITY_AURORA_FIELD) over the acting unit's whole side, reserves included
/// for the party.
void BattleStaging_PlanAuroraField(struct BattlePlan *plan) {
  u16 ids[10];
  s32 count;
  s32 i;

  if (plan->unitId <= ALLY_MAX) {
    count = BattleParty_PrepareMembers(ids);
    count += BattleParty_PrepareReserveOwners(&ids[count]);
  } else {
    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_ENEMIES, ids);
  }
  for (i = 0; i < count; i++) {
    plan->targetIds[i] = ids[i];
    plan->targetOffset[i] = 0;
    plan->targetMultiplier[i] = 1;
    plan->targetCritical[i] = 0;
    plan->targetHit[i] = 1;
  }
  plan->targetCount = count;
  plan->abilityId = ABILITY_AURORA_FIELD;
  plan->battleEffect =
      EFFECT_ON_DOWNED | 2 << EFFECT_VARIANT_SHIFT | BATTLE_EFFECT_REVERSE_STAR;
  plan->hauntDamage = 0;
}

/// Sends every battler back to its spot.
void BattleParty_ReturnAllToSpots(void) {
  u16 ids[UNIT_MAX];
  s32 count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, ids);
  s32 i;

  for (i = 0; i < count; i++)
    BattleMotion_ReturnToSpot(ids[i]);
}

/// Plays one action of the turn after delay frames: an enemy picks its ability
/// now, the action is built and staged, and every unit returns to its spot.
/// Returns 1 when the party got away, 0 otherwise, and -1 when the unit is
/// already down.
s32 BattleStaging_DispatchAction(struct BattleActionRecord *request,
                                 s32 delay) {
  struct BattleBackgroundState *view;
  struct BattleState *battle;
  struct BattleCamera *camera;
  struct Unit *unit;
  u16 actingUnit[2];
  s32 fled;
  s32 result;

  battle = Ram_HeapSlots->battleState;
  fled = 0;
  if (request->unitId == BATTLE_UNIT_LIST_END)
    return 0;
  unit = Unit_Get(request->unitId);
  if (unit->hp == 0)
    return -1;
  battle->actingAnimation = POSE_CAST;
  // An enemy settles its ability now, as it acts.
  if (unit->classId == CLASS_NONE)
    BattleAI_ChooseAction(request, 1);

  // The camera eases back over 60 frames, the background at full brightness,
  // the far depth well past the battlefield.
  view = Ram_HeapSlots->battleBackground;
  view->frames = 60;
  view->keepRange = 0;
  camera = Ram_HeapSlots->cameraState;
  battle->brightness = FIXED_ONE;
  Matrix_Initialize();
  Matrix_SetLookAt(camera->eye, camera->focus);
  {
    s32 ratio = Iwram_DivFX(BATTLE_CAMERA_DISTANCE, FIXED_ONE * 3 / 4);

    Projection_Set(BATTLE_CAMERA_DISTANCE, ratio, BATTLE_FAR_DEPTH);
  }
  if (delay != 0) {
    view->targetYaw = YAW_PARTY_ACTION;
    wait(delay);
  }
  // Build the action, with the acting unit highlighted, and stage it.
  if (request->command == COMMAND_DAEDALUS) {
    BattleStaging_PlanDaedalus((u16 *)request, &battle->plan);
    result = 0;
  } else {
    actingUnit[0] = request->unitId;
    actingUnit[1] = BATTLE_UNIT_LIST_END;
    BattleHighlight_SetModes(actingUnit, HIGHLIGHT_SELECTED);
    result = BattleActionRun_Build(request, &battle->plan);
  }
  if (result == 0) {
    switch (battle->plan.staging) {
    case 0: // an ability with no staging plays nothing
      break;
    case STAGING_MESSAGES:
      BattleStaging_RunMessages(&battle->plan, 0);
      break;
    case STAGING_ATTACK:
      BattleStaging_RunAttack(&battle->plan, 0);
      break;
    case STAGING_BEAST_ATTACK:
      BattleStaging_RunAttack(&battle->plan, 2);
      break;
    case STAGING_SUMMON:
      BattleStaging_RunFullScreen(&battle->plan, STAGING_HIDE_ACTOR);
      break;
    case STAGING_FULL_SCREEN:
      BattleStaging_RunFullScreen(&battle->plan, 0);
      break;
    case STAGING_DAEDALUS:
      BattleStaging_RunFullScreen(&battle->plan,
                                  STAGING_HIDE_ACTOR | STAGING_NO_CAST);
      break;
    case STAGING_CAST:
    case STAGING_BEAST_PSYNERGY:
      BattleStaging_RunPsynergyCast(&battle->plan, CAST_PSYNERGY);
      break;
    case STAGING_DJINNI:
      BattleStaging_RunPsynergyCast(&battle->plan, CAST_DJINNI);
      break;
    case STAGING_ENEMY_CAST:
    case STAGING_BEAST_CAST:
      BattleStaging_RunPsynergyCast(&battle->plan, CAST_ENEMY);
      break;
    case STAGING_ITEM:
      BattleStaging_RunItem(&battle->plan, 0, request);
      break;
    case STAGING_TEAM_STRIKE: {
      u8 swap = battle->plan.targetIds[0];

      battle->events.keepPendingSound = 1;
      battle->plan.targetIds[0] = battle->plan.partnerId;
      battle->skipPlayback = 1;
      BattleStaging_RunPsynergyCast(&battle->plan, CAST_DJINNI);
      battle->skipPlayback = 0;
      battle->plan.targetIds[0] = swap;
      swap = battle->plan.unitId;
      battle->plan.unitId = battle->plan.partnerId;
      battle->plan.partnerId = swap;
      battle->plan.battleEffect = BattleUnit_GetPartyAttackEffect(
          Unit_Get(battle->plan.unitId)->enemyId);
      // A partner striking its own side: "<partner>, stop hitting yourself!"
      // at itself, "<partner> attacks an ally!" at another.
      if (((battle->plan.unitId ^ battle->plan.targetIds[0]) >> 7) == 0) {
        MessageArgument_PushFar(battle->plan.unitId, MESSAGE_ARGUMENT_ACTOR);
        if (battle->plan.unitId == battle->plan.targetIds[0])
          BattleMessage_ShowAndWaitFar((s32)&MsgStopHittingYourself);
        else
          BattleMessage_ShowAndWaitFar((s32)&MsgAttacksAlly);
      }
      BattleStaging_RunAttack(
          &battle->plan,
          battle->plan.abilityId == ABILITY_MELD ? ATTACK_WITH_PARTNER : 0);
      swap = battle->plan.unitId;
      battle->plan.unitId = battle->plan.partnerId;
      battle->plan.partnerId = swap;
      break;
    }
    case STAGING_DJINN_RECOVERY: {
      s32 kept;

      battle->skipPlayback = 1;
      BattleStaging_RunPsynergyCast(&battle->plan, CAST_DJINNI);
      battle->skipPlayback = 0;
      // "Djinn are recovering more quickly!"
      BattleMessage_ShowAndWaitFar((s32)&MsgDjinnRecoverFaster);
      kept = DjinnRecovery_CountDownSide(battle->plan.targetIds[0] > ALLY_MAX);
      // "But no Djinn are in Recovery!"
      if (kept == 0)
        BattleMessage_ShowAndWaitFar((s32)(MsgDjinnRecoverFaster + 1));
      if (kept <= 1)
        BattleMessage_WaitForAdvance();
      break;
    }
    case STAGING_MEGAERA:
      BattleStaging_RunFullScreen(&battle->plan, STAGING_HIDE_ACTOR);
      if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
        break;
      if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0)
        break;
      BattleStaging_PlanHighImpact(&battle->plan);
      BattleStaging_RunPsynergyCast(&battle->plan, CAST_DJINNI);
      break;
    case STAGING_IRIS:
      BattleStaging_RunFullScreen(&battle->plan, STAGING_HIDE_ACTOR);
      BattleStaging_PlanAuroraField(&battle->plan);
      BattleStaging_RunPsynergyCast(&battle->plan, CAST_DJINNI);
      break;
    case STAGING_FLEE:
      if (BattleStaging_RunFlee(&battle->plan) != 0)
        fled = 1;
      if (fled != 0)
        goto finish;
      break;
    }
  } else {
    if (result == ACTION_REPORTED) {
      BattleMessage_WaitForAdvance();
      wait(3);
    }
    BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  }

  // The action's end, then every unit back to its spot. A party that fled
  // skips this and the action's mark.
  BattleMotion_DestroyAllSlotObjects();
  BattleUnit_ProcessActionEnd(&battle->plan);
  BattleObject_Put();
  BattlePlacement_RelayoutEnemies();
  BattleParty_ReturnAllToSpots();
  request->unitId = BATTLE_UNIT_LIST_END;
finish:
  BattleBackground_SetMode(BATTLE_BACKGROUND_ZOOMED,
                           (Ram_HeapSlots->battleState)->background, 0);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  return fled;
}

/// Fills a battle effect command from an action run: its kind and strength, the
/// user and the targets still standing, or all of them for an effect that
/// revives.
void BattleStaging_BuildTargetList(struct BattlePlan *plan,
                                   struct BattlePresentationState *effect) {
  s16 *outputTarget;
  u32 flags;
  s32 livingTargets;
  s32 targetIndex;
  u8 *inputTarget;

  livingTargets = 0;
  effect->prelude = 0;
  flags = plan->battleEffect;
  effect->kind = flags & EFFECT_ID_MASK;
  effect->variant = (flags & EFFECT_VARIANT_MASK) >> EFFECT_VARIANT_SHIFT;
  effect->actor = plan->unitId;
  targetIndex = 0;
  if (targetIndex < plan->targetCount) {
    inputTarget = plan->targetIds;
    outputTarget = effect->actors;
    do {
      if (BattleUnit_IsListed(*inputTarget) &&
          (Unit_Get(*inputTarget)->hp != 0 ||
           (plan->battleEffect & EFFECT_ON_DOWNED))) {
        *outputTarget++ = *inputTarget;
        livingTargets++;
      }
      targetIndex++;
      inputTarget++;
    } while (targetIndex < plan->targetCount);
  }
  if (livingTargets == 0) {
    effect->actors[0] = plan->targetIds[0];
    livingTargets = 1;
  }
  effect->firstTarget = plan->targetIds[0];
  effect->count = livingTargets;
  effect->range = 1;
}

/// Plays a unit's attempt to run: the party runs off one by one unless the roll
/// failed ("But there's no escape!"); an enemy gets away 7 times in 10. Returns
/// 1 when the party got away.
s32 BattleStaging_RunFlee(struct BattlePlan *plan) {
  s32 completed;
  struct BattleBackgroundState *view = Ram_HeapSlots->battleBackground;
  u32 unitId = plan->unitId;
  struct BattleState *battle = Ram_HeapSlots->battleState;
  s32 escaped = 0;

  completed = 0;
  view->targetYaw = YAW_PARTY_ACTION;
  view->busy = 1;
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);

  if (unitId <= ALLY_MAX) {
    // The roll was made as the commands were gathered: a failed one handed the
    // enemies the turn as if they had struck first.
    escaped = completed;
    if (battle->firstStrike != FIRST_STRIKE_ENEMIES)
      escaped = 1;

    if (!escaped) {
      BattleMessage_ShowAndWaitFar((s32)MsgNoEscape);
      BattleMessage_WaitForAdvance();
    } else {
      s16 unitIds[UNIT_MAX];
      s32 index =
          BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)unitIds) - 1;

      while (index != -1) {
        struct Unit *unit = Unit_Get(unitIds[index]);

        if (unit->stun == 0 && unit->sleep == 0) {
          BattleMotion_SetupEscapeObject(unitIds[index]);
          wait(8);
        }
        index--;
      }
      completed = 1;
    }
  } else if ((Random_Below(10)) <= 6) {
    s16 runner[2];

    runner[0] = plan->unitId;
    runner[1] = BATTLE_UNIT_LIST_END;
    BattleMotion_SetupEscapeObject(runner[0]);
    wait(8);
    BattleObject_RemoveFromLists(plan->unitId);
    BattleObject_MarkForRelease(plan->unitId);
  } else {
    BattleMessage_ShowAndWaitFar((s32)MsgNoEscape);
    BattleMessage_WaitForAdvance();
  }

  view->busy = 0;
  return completed;
}

/// Plays a summon or a full-screen Psynergy: the unit glows in its element and
/// the other side fades out while the effect plays, then everyone fades back
/// in. STAGING_HIDE_ACTOR hides the unit too, as the summons do.
s32 BattleStaging_RunFullScreen(struct BattlePlan *plan, s32 mode) {
  u16 units[UNIT_MAX];
  struct BattlePresentationState state;
  u32 actor;
  u32 target;
  u32 count;
  u32 i;
  u32 kept;
  struct MotionObject *object;

  // The camera turns to the acting unit's side for a summon, else to the usual
  // view.
  BattleStaging_BuildTargetList(plan, &state);
  actor = plan->unitId;
  target = plan->targetIds[0];
  if (plan->battleEffect & EFFECT_SUMMON) {
    struct BattleBackgroundState *view = Ram_HeapSlots->battleBackground;

    if (actor <= ALLY_MAX)
      view->targetYaw = YAW_PARTY_ACTION;
    else
      view->targetYaw = YAW_BEHIND_ENEMIES;
    view->frames = 60;
  } else {
    struct BattleBackgroundState *view = Ram_HeapSlots->battleBackground;
    s32 yaw;

    yaw = YAW_ENEMY_ACTION;
    if (actor <= ALLY_MAX)
      yaw = YAW_PARTY_ACTION;
    if (view->targetYaw != yaw)
      view->targetYaw = yaw;
  }

  // The unit raises its hand and glows; the side not targeted fades out.
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  PartyStatus_DrawFar(Ram_HeapSlots->battleState->partyStatus &
                      ~PARTY_STATUS_SHOW_PP);
  object = BattleObject_Get(actor)->object;
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  i = 0;
  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, units);
  for (; i != count; i++) {
    if (units[i] == BATTLE_UNIT_REMOVED)
      continue;
    if ((mode & STAGING_NO_CAST) || units[i] != actor) {
      u32 unit = units[i];

      if ((target <= ALLY_MAX) != (unit <= ALLY_MAX))
        BattleHighlight_SetSpriteMode(unit, SPRITE_SEMI_TRANSPARENT);
    } else {
      Actor_SelectAnimationFar((struct Actor *)object,
                               Ram_HeapSlots->battleState->actingAnimation);
    }
  }
  if (!(mode & STAGING_NO_CAST)) {
    Sound_PlayFar(SE_CAST);
    ElementGlow_Play(state.actor, plan->element, GLOW_STAR_FIELD, 0);
  }
  if (mode & STAGING_HIDE_ACTOR)
    BattleHighlight_SetSpriteMode(actor, SPRITE_SEMI_TRANSPARENT);
  for (i = 0; i != BLEND_WEIGHT_MAX; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
    wait(1);
  }

  // A failed cast: "blocked" (sealed) or too little PP, shown at once.
  if (plan->failure != ACTION_FAILURE_NONE) {
    if (plan->failure == ACTION_FAILURE_SEALED) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->unitId);
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgCastLacksPp);
    }
    BattlePlayback_RunAll();
    BattleBackground_RestorePalette();
  } else {
    // Only the target side stays on the field.
    kept = 0;
    for (i = 0; i < count; i++) {
      u32 unit = units[i];

      if (unit == actor) {
        if (!(mode & STAGING_HIDE_ACTOR))
          units[kept++] = actor;
      } else if ((target <= ALLY_MAX) == (unit <= ALLY_MAX)) {
        units[kept++] = unit;
      }
    }
    units[kept] = BATTLE_UNIT_LIST_END;
    BattleActor_SpawnObjectsForList((s16 *)units, 0);
    for (i = 0; i < plan->targetCount; i++)
      units[i] = plan->targetIds[i];
    units[i] = BATTLE_UNIT_LIST_END;
    // Each target's colour modes are saved beside the effect command.
    for (i = 0; i != state.count; i++) {
      struct Sprite *animation =
          Actor_GetSprite(BattleObject_Get(state.actors[i])->object, 0);
      u32 child;

      for (child = 0; child != animation->count - 1; child++)
        state.params[i][child] = animation->entries[child]->colorMode;
    }
    // The side the battle effect plays on: the targets'.
    if (plan->battleEffect & EFFECT_SUMMON) {
      if (target <= ALLY_MAX)
        state.side = EFFECT_SIDE_PARTY;
      else
        state.side = EFFECT_SIDE_ENEMIES;
    } else {
      if (plan->targetIds[0] <= ALLY_MAX)
        state.side = EFFECT_SIDE_PARTY;
      else
        state.side = EFFECT_SIDE_ENEMIES;
    }
    if (plan->battleEffect & EFFECT_OTHER_SIDE)
      state.side ^= 1;
    // The battle effect runs to its end while playback, a task, plays each
    // target's outcome from the frame the blow lands.
    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (plan->battleEffect & EFFECT_SUMMON)
      BattleEffect_RunSummonFar((s32 *)&state);
    else if (plan->battleEffect & EFFECT_BY_ID_RANGE)
      BattleEffect_RunByIdRangeFar(&state);
    else
      BattleEffect_RunFromTableFar(&state);
    BattlePlayback_WaitForReady();
  }

  // Everyone back: the faded units fade in over 16 frames and every colour
  // mode is cleared.
  BattleObject_Put();
  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, units);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (i = 0; i != count; i++) {
    u32 unit;

    if (units[i] == BATTLE_UNIT_REMOVED)
      continue;
    // Gaia's caster stays out of the fade back in, and so, as kind
    // SUMMON_EFFECT_KIRIN, does Kirin's summoner: it is back at once where
    // Tiamat's fades in with the rest.
    if (!(mode & STAGING_NO_CAST) && units[i] == actor &&
        state.kind == BATTLE_EFFECT_GAIA)
      continue;
    unit = units[i];
    if ((target <= ALLY_MAX) != (unit <= ALLY_MAX))
      BattleHighlight_SetSpriteMode(unit, SPRITE_SEMI_TRANSPARENT);
  }
  for (i = 0; i != BLEND_WEIGHT_MAX; i++) {
    REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
    wait(1);
  }
  for (i = 0; i != count; i++)
    BattleHighlight_SetSpriteMode(units[i], SPRITE_OPAQUE);
  BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
  wait(1);
  return 0;
}

/// Plays an action with no battle effect (STAGING_MESSAGES): each target's
/// outcome is applied and its messages shown in turn. With no targets, it only
/// waits for the message to advance.
UNREAD_RESULT BattleStaging_RunMessages(struct BattlePlan *plan, s32 unused) {
  s32 targetIndex;

  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  if (plan->targetCount == 0) {
    BattleMessage_WaitForAdvance();
  } else {
    for (targetIndex = 0; targetIndex < plan->targetCount; targetIndex++) {
      BattleAbility_ApplyToTarget(plan, targetIndex);
      BattlePlayback_RunAll();
    }
  }
  wait(1);
}
