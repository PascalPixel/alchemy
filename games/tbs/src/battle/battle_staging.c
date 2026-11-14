/// @file
/// An action played: its targets, and how an attack, a cast, an item, a
/// summon or a flight is staged.
// Part of battle.c.

#include "battle_internal.h"

/// Plays one action of the turn, after delay frames facing the party: the
/// action is built and staged, its end effects run and every unit returns to
/// its spot. Returns 1 when the party got away, -1 when the unit is already
/// down, else 0.
s32 BattleStaging_DispatchAction(struct BattleAction *request, s32 delay) {
  struct BattleBackgroundWork *view;
  struct BattleWork *battle;
  struct BattleCamera *camera;
  struct Unit *unit;
  u16 actingUnit[2];
  u16 unitIds[UNIT_MAX];
  s32 fled;
  s32 result;
  s32 unitCount;
  s32 unitIndex;

  fled = 0;
  if (request->unitId == BATTLE_UNIT_LIST_END)
    return 0;
  unit = Unit_GetFar(request->unitId);
  if (unit->hp == 0)
    return -1;
  // An enemy settles its ability now, as it acts.
  if (unit->classId == CLASS_NONE)
    BattleAI_ChooseAction(request, 1);

  // The camera eases back over 60 frames, the background at full brightness,
  // the far depth well past the battlefield.
  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  view->frames = 60;
  battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  view->flag = 0;
  battle->brightness = Q16_ONE;
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  Transform_Reset();
  Transform_SetLookAt(camera->eye, camera->focus);
  Projection_Set(BATTLE_CAMERA_DISTANCE,
                 Iwram_RatioMulQ14(BATTLE_CAMERA_DISTANCE, Q16_ONE * 3 / 4),
                 0x7fff0000);
  if (delay != 0) {
    view->targetYaw = YAW_PARTY_ACTION;
    wait(delay);
  }
  // Build the action, with the acting unit highlighted, and stage it.
  actingUnit[0] = request->unitId;
  actingUnit[1] = BATTLE_UNIT_LIST_END;
  BattleHighlight_SetModes(actingUnit, HIGHLIGHT_SELECTED);
  result = BattleActionRun_Build(request, &battle->actionRun);
  if (result == 0) {
    switch (battle->actionRun.staging) {
    case STAGING_MESSAGES:
      BattleStaging_RunActorEntries(&battle->actionRun, 0);
      break;
    case STAGING_ATTACK:
      BattleStaging_RunAttack(&battle->actionRun, 0);
      break;
    case STAGING_SUMMON:
      BattleStaging_HideField(&battle->actionRun, STAGING_HIDE_ACTOR);
      break;
    case STAGING_FULL_SCREEN:
      BattleStaging_HideField(&battle->actionRun, 0);
      break;
    case STAGING_CAST:
      BattleStaging_RunPsynergyCast(&battle->actionRun, CAST_PSYNERGY);
      break;
    case STAGING_DJINNI:
      BattleStaging_RunPsynergyCast(&battle->actionRun, CAST_DJINNI);
      break;
    case STAGING_ENEMY_CAST:
      BattleStaging_RunPsynergyCast(&battle->actionRun, CAST_ENEMY);
      break;
    case STAGING_ITEM:
      BattleStaging_RunItem(&battle->actionRun, 0, request);
      break;
    case STAGING_FLEE:
      if (BattleStaging_RunFlee(&battle->actionRun) != 0)
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
  BattleObject_DestroyAll();
  BattleUnit_ProcessActionEnd(&battle->actionRun);
  BattleObject_Put();
  unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
    BattleMotion_ReturnToSpot(unitIds[unitIndex]);
  request->unitId = BATTLE_UNIT_LIST_END;
finish:
  BattleBackground_SetMode(BATTLE_BACKGROUND_ZOOMED, gBattleWork->background,
                           0);
  return fled;
}

/// Fills a battle effect command from an action run: its kind and strength, the
/// user and the targets still standing, or all of them for an effect that
/// revives.
UNREAD_RESULT BattleStaging_BuildTargetList(struct BattleActionRun *plan,
                                            struct BattleEffectParams *args) {
  s32 count = 0;
  s32 i;

  args->prelude = 0;
  args->kind = plan->battleEffect & EFFECT_ID_MASK;
  args->variant =
      (plan->battleEffect & EFFECT_VARIANT_MASK) >> EFFECT_VARIANT_SHIFT;
  args->user = plan->unitId;
  for (i = 0; i < plan->targetCount; i++) {
    if (Unit_GetFar(plan->targetIds[i])->hp != 0 ||
        (plan->battleEffect & EFFECT_ON_DOWNED))
      args->targets[count++] = plan->targetIds[i];
  }
  if (count == 0) {
    args->targets[0] = plan->targetIds[0];
    count = 1;
  }
  args->firstTarget = plan->targetIds[0];
  args->targetCount = count;
  args->range = 1;
}

/// Plays a unit's attempt to run: the party runs off one by one unless the roll
/// failed ("But there's no escape!"); an enemy gets away 7 times in 10. Returns
/// 1 when the party got away.
s32 BattleStaging_RunFlee(struct BattleActionRun *plan) {
  struct BattleBackgroundWork *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  s32 completed = 0;
  // Default: 0 until the party branch copies completed.
  s32 partyMode = 0;

  view->targetYaw = YAW_PARTY_ACTION;
  view->busy = 1;
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);

  // The party: each party member that can move runs, the last listed first,
  // eight frames apart.
  if (plan->unitId <= ALLY_MAX) {
    partyMode = completed;

    if (battle->firstStrike != FIRST_STRIKE_ENEMIES) {
      partyMode = 1;
    }

    if (!partyMode) {
      BattleMessage_ShowAndWaitFar(MsgNoEscape);
      BattleMessage_WaitForAdvance();
    } else {
      s16 unitIds[UNIT_MAX];
      s32 index =
          BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)unitIds) - 1;

      while (index != -1) {
        struct Unit *unit = Unit_GetFar(unitIds[index]);

        if (unit->stun == 0 && unit->sleep == 0) {
          BattleMotion_SetupEscapeObject(unitIds[index]);
          wait(8);
        }
        index--;
      }
      wait(22);
      completed = 1;
    }
  } else if (((Random16() * 10) >> 16) <= 6) {
    // An enemy: a roll of 0 to 9, 7 in 10 away.
    s16 event[2];

    event[0] = plan->unitId;
    event[1] = BATTLE_UNIT_LIST_END;
    BattleMotion_SetupEscapeObject(event[0]);
    wait(8);
    BattleObject_RemoveFromLists(plan->unitId);
    BattleObject_MarkForRelease(plan->unitId);
  } else {
    BattleMessage_ShowAndWaitFar(MsgNoEscape);
    BattleMessage_WaitForAdvance();
  }

  view->busy = 0;
  return completed;
}

/// Plays a summon or a full-screen Psynergy: the unit glows in its element and
/// the other side fades out while the effect plays, then everyone fades back
/// in. STAGING_HIDE_ACTOR hides the unit too, as the summons do.
s32 BattleStaging_HideField(struct BattleActionRun *plan, s32 mode) {
  u16 units[UNIT_MAX];
  struct BattleStagingWork work;
  u32 actor;
  u32 target;
  u32 count;
  s32 i;
  s32 n;
  u32 kept;
  struct Actor *object;
  // UB: indexing past scalar gBattleWork reaches another heap slot.
#ifndef UBFIX
  void **slots = (void **)&gBattleWork;
#endif

  // The camera turns to the acting unit's side for a staged effect, else to the
  // usual view.
  BattleStaging_BuildTargetList(plan, &work.effect);
  actor = plan->unitId;
  target = plan->targetIds[0];
  if (plan->battleEffect & EFFECT_SUMMON) {
    struct BattleBackgroundWork *view;
    s32 yaw;
#ifdef UBFIX
    view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    yaw = YAW_PARTY_ACTION;
    if (actor > ALLY_MAX)
      yaw = YAW_BEHIND_ENEMIES;
    view->targetYaw = yaw;
    view->frames = 60;
  } else {
    struct BattleBackgroundWork *view;
    s32 yaw;
#ifdef UBFIX
    view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    yaw = YAW_ENEMY_ACTION;
    if (actor <= ALLY_MAX)
      yaw = YAW_PARTY_ACTION;
    if (view->targetYaw != yaw)
      view->targetYaw = yaw;
  }

  // The unit raises its hand and glows; the side not targeted fades out.
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  PartyStatus_DrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);
  object = BattleObject_Get(actor)->actor;
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, units);
  for (i = 0; i != count; i++) {
    if (units[i] == BATTLE_UNIT_REMOVED)
      continue;
    if (units[i] != actor) {
      if ((target <= ALLY_MAX) != (units[i] <= ALLY_MAX))
        BattleHighlight_SetSpriteMode(units[i], SPRITE_SEMI_TRANSPARENT);
    } else {
      Actor_SelectAnimationFar(object, POSE_CAST);
    }
  }
  // The sound of a cast, as the caster raises its hand.
  Sound_PlayFar(SE_CAST);
  ElementGlow_Play(work.effect.user, plan->element, GLOW_STAR_FIELD, 0);
  if (mode & STAGING_HIDE_ACTOR)
    BattleHighlight_SetSpriteMode(actor, SPRITE_SEMI_TRANSPARENT);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = BLEND_ALPHA(16 - i, 16);
    wait(1);
  }
  // A failed cast: "blocked" (sealed) or too little PP, shown at once.
  if (plan->failure != ACTION_FAILURE_NONE) {
    if (plan->failure == ACTION_FAILURE_SEALED) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->unitId);
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgCastLacksPp);
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
    BattleObject_SpawnObjectsForList(units, 0);
    for (n = 0; n < plan->targetCount; n++)
      units[n] = plan->targetIds[n];
    units[n] = BATTLE_UNIT_LIST_END;
    // Each target's colour modes are saved beside the effect command.
    // XXX (no evidence): what for; nothing reads them back.
    for (i = 0; i != work.effect.targetCount; i++) {
      struct Sprite *animation;
      u32 child;

      animation =
          GetMotionRecord(BattleObject_Get(work.effect.targets[i])->actor, 0);
      for (child = 0; child != animation->count - 1; child++)
        work.savedAnimationParameters[i][child] =
            animation->entries[child]->colorMode;
    }
    // The side the battle effect plays on: the targets'.
    if (plan->battleEffect & EFFECT_SUMMON) {
      if (target > ALLY_MAX)
        work.effect.side = EFFECT_SIDE_ENEMIES;
      else
        work.effect.side = EFFECT_SIDE_PARTY;
    } else {
      if (plan->targetIds[0] <= ALLY_MAX)
        work.effect.side = EFFECT_SIDE_PARTY;
      else
        work.effect.side = EFFECT_SIDE_ENEMIES;
    }
    if (plan->battleEffect & EFFECT_OTHER_SIDE)
      work.effect.side ^= 1;
    // The battle effect runs to its end while playback, a task, plays
    // each target's outcome from the frame the blow lands.
    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (plan->battleEffect & EFFECT_SUMMON)
      BattleEffect_RunSummonFar(&work.effect);
    else if (plan->battleEffect & EFFECT_BY_ID_RANGE)
      BattleEffect_RunByIdRangeFar(&work.effect);
    else
      BattleEffect_RunFromTableFar(&work.effect);
    BattlePlayback_WaitForReady();
  }

  // Everyone back: the faded units fade in over 16 frames and every colour
  // mode is cleared.
  BattleObject_Put();
  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, units);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (i = 0; i != count; i++) {
    if (units[i] == BATTLE_UNIT_REMOVED || units[i] == actor)
      continue;
    if ((target <= ALLY_MAX) != (units[i] <= ALLY_MAX))
      BattleHighlight_SetSpriteMode(units[i], SPRITE_SEMI_TRANSPARENT);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = BLEND_ALPHA(i, 16);
    wait(1);
  }
  for (i = 0; i != count; i++)
    BattleHighlight_SetSpriteMode(units[i], SPRITE_OPAQUE);
  BattleCamera_SetView(0, 0, 0, 100);
  wait(1);
  return 0;
}

/// Clears the highlights, then applies the action to each target in turn and
/// plays its events; with no target, only waits for the player.
UNREAD_RESULT BattleStaging_RunActorEntries(struct BattleActionRun *plan,
                                            s32 unused) {
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

/// Plays an attack: the attacker runs up to its first target and the blow
/// lands. A miss makes the target hop back ("nimbly dodges the blow"); a
/// critical hit zooms in on the pair with only the two of them shown.
s32 BattleStaging_RunAttack(struct BattleActionRun *plan, s32 mode) {
  struct BattleStagingWork work;
  struct Actor *actor;
  struct Sprite *animation;
  struct Unit *unit;
  s16 criticalPair[3];
  s32 critical;
  struct BattleBackgroundWork *view;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 x;
  s32 z;
  s32 frames;
  s32 missed;
  s32 index;
  s32 zoomOnCritical;

  view = gBattleBackgroundWork;
  {
    struct BattleActionRun *savedPlan = plan;

    /* FAKEMATCH(dead): plan cleared and restored under tests that emit nothing;
     * without them, 17 of 289 instructions differ in tbs-en. */
    if (view)
      plan = NULL;
    if (view)
      plan = savedPlan;
  }

  // The camera turns halfway to the attacker's view.
  actor = BattleObject_Get(plan->unitId)->actor;
  z = actor->z.fixed;
  x = actor->x.fixed;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - TRIG_TURN / 8;
  if (plan->unitId > ALLY_MAX)
    adjusted = angle + TRIG_TURN * 3 / 8;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - YAW_PARTY_ACTION) / 2 + YAW_PARTY_ACTION;

  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    wait(5);
  } else {
    view->targetYaw = facingAngle;
    wait(10);
  }

  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  BattleStaging_BuildTargetList(plan, &work.effect);
  // Severe Blow drops the status window's PP row while it plays.
  if (work.effect.kind == EFFECT_ID_SEVERE_BLOW)
    PartyStatus_DrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);

  unit = Unit_GetFar(work.effect.user);
  Unit_GetFar(work.effect.targets[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  // The run up: as long as the attacker's POSE_LEAP, with its class's hop
  // height (16.16).
  animation = GetMotionRecord(BattleObject_Get(plan->unitId)->actor, 0);
  frames = SpriteAnimation_SumCommandLengthsFar(
      animation->entries[0]->animationId, 2, 1);
  BattleMotion_ApproachTarget(work.effect.user, work.effect.targets[0], frames,
                              px(BattleSprite_GetAttackHop(unit->id)));
  Actor_SetAnimationSpeedFar(BattleObject_Get(work.effect.user)->actor,
                             ANIMATION_STEP_NORMAL);
  BattleObject_Get(work.effect.targets[0]);

  if ((u16)work.effect.targets[0] <= ALLY_MAX)
    work.effect.side = EFFECT_SIDE_PARTY;
  else
    work.effect.side = EFFECT_SIDE_ENEMIES;

  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = BATTLE_WININ;
  REG_WINOUT = BATTLE_WINOUT;
  REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;

  // A miss: after 10 frames the target hops back; 16 frames later the dodge
  // is shown at once.
  if (missed != 0) {
    wait(10);
    BattleMotion_HopBack(work.effect.targets[0]);
    wait(2);
    wait(4);
    wait(10);
    BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->targetIds[0]);
    BattlePlayback_Push(PLAYBACK_MESSAGE, MsgNimblyDodgesBlow);
    BattlePlayback_RunAll();
    BattleMotion_ReturnToSpot(work.effect.targets[0]);
  } else {
    // A hit; a critical one zooms in on the pair over the rest of the run up.
    zoomOnCritical = 0;
    work.effect.prelude = 0;
    if (plan->battleEffect != 0)
      work.effect.prelude = 1;

    if (critical != 0) {
      work.effect.kind += EFFECT_ID_CRITICAL_OFFSET;
      zoomOnCritical = 1;
      view->flag = 1;
      criticalPair[0] = work.effect.user;
      criticalPair[1] = work.effect.firstTarget;
      criticalPair[2] = BATTLE_UNIT_LIST_END;
      BattleObject_SpawnObjectsForList(criticalPair, 0);
    }

    // The battle effect starts 8 frames before the run up ends, at least one
    // frame after it began.
    frames -= 8;
    if (frames <= 0)
      frames = 1;
    {
      s32 first;
      s32 second;

      for (index = 0; index != frames; index++) {
        if (zoomOnCritical != 0) {
          first = work.effect.user;
          second = work.effect.firstTarget;
          BattleCamera_SetViewAtPairMidpoint(first, second,
                                             index * 30 / frames + 100);
        }
        wait(1);
      }
    }

    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (work.effect.kind != 0) {
      if (plan->battleEffect & EFFECT_BY_ID_RANGE)
        BattleEffect_RunByIdRangeFar(&work.effect);
      else
        BattleEffect_RunFromTableFar(&work.effect);
    }
    BattlePlayback_WaitForReady();
    // After a critical hit every unit is shown again and the zoom ends.
    if (critical != 0) {
      view->flag = 0;
      BattleUnit_ListAllAndSubmit();
      BattleCamera_SetView(0, 0, 0, 100);
    }
    BattleMotion_ReturnToSpot(work.effect.targets[0]);
  }
  BattleMotion_ReturnToSpot(work.effect.user);
  return 0;
}

/// Unused: a plainer attack run, with no hop, no critical zoom and no dodge
/// message.
UNUSED s32 BattleStaging_RunPlainAttack(struct BattleActionRun *input,
                                        s32 flags) {
  struct BattleStagingWork work;
  struct BattleActionRun *savedInput;
  struct Actor *actor;
  struct Sprite *animation;
  struct BattleBackgroundWork *view;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 x;
  s32 z;
  s32 frames;
  s32 missed;

  view = gBattleBackgroundWork;
  savedInput = input;
  actor = BattleObject_Get(savedInput->unitId)->actor;
  z = actor->z.fixed;
  x = actor->x.fixed;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - TRIG_TURN / 8;
  if (savedInput->unitId > ALLY_MAX)
    adjusted = angle + TRIG_TURN * 3 / 8;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - YAW_PARTY_ACTION) / 2 + YAW_PARTY_ACTION;

  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    wait(5);
  } else {
    view->targetYaw = facingAngle;
    wait(20);
  }

  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  BattleStaging_BuildTargetList(savedInput, &work.effect);
  Unit_GetFar(work.effect.user);
  Unit_GetFar(savedInput->targetIds[0]);

  missed = flags & 2;
  animation = GetMotionRecord(BattleObject_Get(savedInput->unitId)->actor, 0);
  frames = SpriteAnimation_SumCommandLengthsFar(
      animation->entries[0]->animationId, 2, 1);
  BattleMotion_ApproachTarget(work.effect.user, savedInput->targetIds[0],
                              frames, 0);
  Actor_SetAnimationSpeedFar(BattleObject_Get(work.effect.user)->actor,
                             ANIMATION_STEP_NORMAL);
  BattleObject_Get(savedInput->targetIds[0]);

  if (savedInput->targetIds[0] <= ALLY_MAX)
    work.effect.side = EFFECT_SIDE_PARTY;
  else
    work.effect.side = EFFECT_SIDE_ENEMIES;
  if (missed != 0) {
    wait(10);
    BattleMotion_HopBack(savedInput->targetIds[0]);
    wait(2);
    wait(4);
    wait(10);
    BattleMotion_ReturnToSpot(savedInput->targetIds[0]);
  } else {
    BattleEffect_RunByIdRangeFar(&work.effect);
    BattlePlayback_RunAll();
    BattleMotion_ReturnToSpot(savedInput->targetIds[0]);
  }
  BattleMotion_ReturnToSpot(work.effect.user);
  return 0;
}

/// Plays an item's use: the user raises the item and, for one with a battle
/// effect, the screen dims while it plays. Then the item is spent as its use
/// type says, and later item commands of the same party member this turn follow
/// their items to their new slots.
s32 BattleStaging_RunItem(struct BattleActionRun *plan, s32 unused,
                          struct BattleAction *selection) {
  struct BattleStagingWork work;
  struct Actor *actor;
  s32 targetOrFadeIndex;
  s32 inventoryEntry;
  struct Unit *unit;
  struct ItemDefinition *item;
  struct BattleBackgroundWork *view = gBattleBackgroundWork;
  s32 targetYaw;

  // The camera turns to the user's side, and the user raises the item.
  // XXX (no evidence): whether > 4 is a slip for > ALLY_MAX; only Jenna, Sheba
  // and Piers would take the enemies' view, and none of them fights here.
  if (plan->unitId > 4)
    targetYaw = YAW_ENEMY_ACTION;
  else
    targetYaw = YAW_PARTY_ACTION;
  if (view->targetYaw != targetYaw)
    view->targetYaw = targetYaw;
  BattleStaging_BuildTargetList(plan, &work.effect);
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  actor = BattleObject_Get(work.effect.user)->actor;
  Actor_SelectAnimationFar(actor, POSE_CAST);
  Actor_SetAnimationSpeedFar(actor, ANIMATION_STEP_NORMAL);
  if (plan->targetIds[0] <= ALLY_MAX)
    work.effect.side = EFFECT_SIDE_PARTY;
  else
    work.effect.side = EFFECT_SIDE_ENEMIES;

  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.effect.targetCount;
       targetOrFadeIndex++) {
    struct Sprite *animation = GetMotionRecord(
        BattleObject_Get(work.effect.targets[targetOrFadeIndex])->actor, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetOrFadeIndex][entryIndex] =
          animation->entries[entryIndex]->colorMode;
  }

  // An item with a battle effect dims the background, then plays it; one with
  // none waits a second.
  Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
  if (work.effect.kind != 0) {
    s32 fade;

    targetOrFadeIndex = 0;
    fade = 0;
    do {
      struct BattleWork *battle = gBattleWork;

      if (targetOrFadeIndex < 20) {
        u16 *palette = battle->palette;
        s32 value = Q16_ONE - fade;

        battle->brightness = value;
        Graphics_ScaleRgb555Clamped(palette, (u16 *)(BG_PLTT + 6 * 16), value,
                                    128);
      }
      wait(1);
      targetOrFadeIndex++;
      fade += Q16_ONE / 60;
    } while (targetOrFadeIndex < 20);
    if (plan->battleEffect & EFFECT_BY_ID_RANGE)
      BattleEffect_RunByIdRangeFar(&work.effect);
    else
      BattleEffect_RunFromTableFar(&work.effect);
  } else {
    wait(60);
  }
  BattlePlayback_WaitForReady();
  Actor_SelectAnimationFar(actor, POSE_STANDING);
  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.effect.targetCount;
       targetOrFadeIndex++)
    BattleMotion_ReturnToSpot(work.effect.targets[targetOrFadeIndex]);

  // Spend the item, by its use type.
  unit = Unit_GetFar(selection->unitId);
  inventoryEntry = unit->inventory[selection->parameter];
  item = Item_GetDirectFar(inventoryEntry);
  if (item->useType == ITEM_USE_CONSUMED) {
    s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
    s32 index = selection->parameter;
    if (result == INVENTORY_REMOVE_LAST) {
      struct BattleWork *battle = gBattleWork;
      u32 row;

      for (row = 0; row < BATTLE_ACTION_CAPACITY; row++) {
        if (battle->actions[row].command == COMMAND_ITEM &&
            battle->actions[row].unitId == selection->unitId) {
          if (battle->actions[row].parameter == index)
            battle->actions[row].parameter = -1;
          else if (battle->actions[row].parameter > index)
            battle->actions[row].parameter--;
        }
      }
    }
  } else if (item->useType == ITEM_USE_MAY_BREAK) {
    // One time in eight it breaks, shown at once.
    if ((Battle_RandomFar() & 7) == 0) {
      BattlePlayback_Push(PLAYBACK_ITEM_NAME,
                          unit->inventory[selection->parameter]);
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgUsedItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattlePlayback_RunAll();
    }
  } else if (item->useType == ITEM_USE_EMPTIES) {
    // Hermes' Water becomes one Empty Bottle, the slot's other bits dropped;
    // any other such item stays as it was.
    if ((inventoryEntry & INVENTORY_ITEM_ID_MASK) == ITEM_HERMES_WATER)
      inventoryEntry = ITEM_EMPTY_BOTTLE;
    unit->inventory[selection->parameter] = inventoryEntry;
  }
  return 0;
}

/// Draws every sprite of an actor in a colour mode, its first layer in
/// colorMode and the others plain.
UNREAD_RESULT BattleObject_SetColorMode(struct Actor *actor, s32 colorMode) {
  s32 recordIndex = 0;
  s32 childIndex;
  s32 entryCount;
  struct Sprite *record;
  struct SpriteAnimation *child;
  struct SpriteAnimation **children;
  u8 lastPictureMask = 0xff;
  u8 previousPicture;

  while ((record = GetMotionRecord(actor, recordIndex)) != NULL) {
    children = &record->entries[1];
    child = record->entries[0];
    previousPicture = child->picture;
    child->picture = previousPicture | lastPictureMask;
    entryCount = record->count;
    child->colorMode = colorMode;
    for (childIndex = 1; childIndex < entryCount; childIndex++) {
      child = *children++;
      child->colorMode = 0;
      child->picture = 0xff;
    }
    recordIndex++;
  }
}

/// Plays Psynergy, a Djinni or an enemy's cast: the caster raises its hand and
/// glows in its element, then the cast fails or its battle effect plays, and
/// the targets return to their spots.
s32 BattleStaging_RunPsynergyCast(struct BattleActionRun *plan, s32 mode) {
  struct BattleStagingWork work;
  struct BattleBackgroundWork *view = gBattleBackgroundWork;
  struct Actor *object;
  s32 isDjinni;
  s32 targetIndex;

  // The camera turns toward the targets from the caster's side, or across.
  if (plan->battleEffect & EFFECT_VIEW_ACROSS) {
    view->targetYaw =
        plan->unitId <= ALLY_MAX ? YAW_ENEMY_ACTION : YAW_BEHIND_ENEMIES;
    view->frames = 60;
  } else {
    struct Actor *actor = BattleObject_Get(plan->unitId)->actor;
    s32 angle = (u16)ArcTan2(actor->x.fixed, actor->z.fixed);
    s32 targetYaw;

    if (plan->unitId <= ALLY_MAX)
      targetYaw = angle - TRIG_TURN * 3 / 32;
    else
      targetYaw = angle + TRIG_TURN * 3 / 32;
    targetYaw = (s16)targetYaw;
    targetYaw +=
        ((plan->unitId <= ALLY_MAX ? YAW_PARTY_ACTION : YAW_ENEMY_ACTION) -
         targetYaw) *
        3 / 4;
    if (!((plan->unitId <= ALLY_MAX) ^ (plan->targetIds[0] <= ALLY_MAX)))
      targetYaw =
          plan->unitId <= ALLY_MAX ? TRIG_TURN * 9 / 64 : -(TRIG_TURN * 9 / 64);
    if (view->targetYaw != targetYaw)
      view->targetYaw = targetYaw;
  }
  if (plan->battleEffect & EFFECT_VIEW_REVERSED) {
    view->targetYaw =
        plan->unitId <= ALLY_MAX ? YAW_ENEMY_ACTION : YAW_PARTY_ACTION;
    view->frames = 60;
  }

  // The caster raises its hand and glows in its element; a Djinni plays its
  // prelude instead.
  BattleStaging_BuildTargetList(plan, &work.effect);
  isDjinni = mode & CAST_DJINNI;
  if (isDjinni)
    work.effect.prelude = 1;
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  PartyStatus_DrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);
  object = BattleObject_Get(work.effect.user)->actor;
  Actor_SelectAnimationFar(object, POSE_CAST);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
  Sound_PlayFar(SE_CAST);
  if (mode & CAST_ENEMY)
    ElementGlow_Play(work.effect.user, plan->element, GLOW_STREAKS, 0);
  else if (!isDjinni)
    ElementGlow_Play(work.effect.user, plan->element, GLOW_STAR_FIELD, 0);
  if (plan->targetIds[0] <= ALLY_MAX)
    work.effect.side = EFFECT_SIDE_PARTY;
  else
    work.effect.side = EFFECT_SIDE_ENEMIES;

  for (targetIndex = 0; targetIndex != work.effect.targetCount; targetIndex++) {
    struct Sprite *animation = GetMotionRecord(
        BattleObject_Get(work.effect.targets[targetIndex])->actor, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetIndex][entryIndex] =
          animation->entries[entryIndex]->colorMode;
  }

  // A failed cast is shown at once; otherwise the battle effect plays during
  // playback, and an ability with no battle effect only restores the palette.
  if (plan->failure != ACTION_FAILURE_NONE) {
    if (plan->failure == ACTION_FAILURE_SEALED) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->unitId);
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgCastLacksPp);
    }
    BattlePlayback_RunAll();
    BattleBackground_RestorePalette();
  } else {
    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (work.effect.kind) {
      if (plan->battleEffect & EFFECT_BY_ID_RANGE)
        BattleEffect_RunByIdRangeFar(&work.effect);
      else
        BattleEffect_RunFromTableFar(&work.effect);
    } else {
      BattleBackground_RestorePalette();
    }
    BattlePlayback_WaitForReady();
    Actor_SelectAnimationFar(object, POSE_STANDING);
    for (targetIndex = 0; targetIndex != work.effect.targetCount; targetIndex++)
      BattleMotion_ReturnToSpot(work.effect.targets[targetIndex]);
  }
  return 0;
}

/// Flashes a unit as it is hit, white and back twice in its damage pose.
void BattleMotion_FlashHit(s32 unitId, s32 unused0, s32 unused1) {
  u16 selection[2];
  s32 flash;

  // DEAD: the unit, fetched and not used; the ROM keeps the call.
  Unit_GetFar(unitId);
  Actor_SelectAnimationFar(BattleObject_Get(unitId)->actor, POSE_DAMAGE);
  for (flash = 0; flash < 2; flash++) {
    selection[0] = unitId;
    selection[1] = BATTLE_UNIT_LIST_END;
    PartyStatus_HighlightMembersFar(selection);
    BattleObject_SetColorMode(BattleObject_Get(unitId)->actor,
                              SPRITE_COLOR_WHITE);
    wait(2);

    selection[0] = unitId;
    PartyStatus_HighlightMembersFar(selection);
    BattleObject_SetColorMode(BattleObject_Get(unitId)->actor,
                              BattleObject_GetPalette(unitId));
    wait(2);
  }
  PartyStatus_DrawFar(gBattleWork->partyStatus);
}

/// Takes a unit out of its side's list and this turn's actions, and frees its
/// name letter.
void BattleObject_RemoveFromLists(s32 unitId) {
  struct BattleWork *battle;
  s32 partyIndex;
  u32 i;
  s32 listedUnit;

  // The party's list.
  battle = gBattleWork;
  Unit_GetFar(unitId)->side = BATTLE_SIDE_NONE;
  for (partyIndex = 0;; partyIndex++) {
    if (battle->partyUnits[partyIndex] == unitId) {
      battle->partyUnits[partyIndex] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (battle->partyUnits[partyIndex] == BATTLE_UNIT_LIST_END)
      break;
  }

  // The enemies' list.
  /* FAKEMATCH(dead): a once-only block round the label loop; as the plain loop,
   * 26 of 53 instructions differ in tbs-en. */
  do {
    i = 0;
  again:
    listedUnit = battle->enemyIds[i];
    if (listedUnit == unitId) {
      battle->enemyIds[i] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    i++;
    if (listedUnit == BATTLE_UNIT_LIST_END)
      return;
    goto again;
  } while (0);

removed:
  // Free the letter that told the enemy apart from others of its class.
  BattleEnemy_ReleaseNameLetter(unitId);
  for (i = 0; i < BATTLE_ACTION_CAPACITY; i++) {
    if (battle->actions[i].unitId == unitId)
      battle->actions[i].unitId = BATTLE_UNIT_LIST_END;
  }
}

/// Shows a downed unit: a party member lies down; an enemy turns grey and
/// dissolves a dot at a time.
void BattleMotion_ShowDowned(s32 unitId) {
  void *records[4];
  struct Unit *unit;
  struct Sprite *record;
  struct SpriteAnimation *entry;
  s32 recordCount;

  unit = Unit_GetFar(unitId);
  recordCount = 0;
  while ((record = GetMotionRecord(BattleObject_Get(unitId)->actor,
                                   recordCount)) != 0) {
    if (unit->side != BATTLE_SIDE_ENEMY)
      Sprite_StartAnimationFar(record, POSE_PARTY_HELD);
    else
      Sprite_StartAnimationFar(record, POSE_ENEMY_DOWN);
    recordCount++;
  }

  if (unit->side == BATTLE_SIDE_ENEMY) {
    recordCount = 0;
    while ((record = GetMotionRecord(BattleObject_Get(unitId)->actor,
                                     recordCount)) != 0) {
      entry = record->entries[0];
      records[recordCount] = record;
      entry->colorMode = SPRITE_COLOR_GREY;
      entry->picture = 0xff;
      recordCount++;
    }
    wait(4);
    BattleObject_RemoveFromLists(unitId);
    // The whole dissolve, over 32 frames.
    SpriteDissolve_RunManyFar((u8 **)records, recordCount);
    BattleObject_MarkForRelease(unitId);
  }
}
