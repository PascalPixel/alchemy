/// @file
/// An action played: its targets, and how an attack, a cast, an item, a
/// summon or a flight is staged.
// Part of battle.c.

#include "../battle_internal.h"

/// Plays one action of the round, after delay frames facing the party: an
/// enemy picks its ability now; the turn is built (its targets, hits
/// and damage) and shown the way its staging asks, then the turn's end
/// effects run and every unit returns to its spot. Returns 1 when the
/// party got away, 0 otherwise, and -1 when the actor is already down.
/// Called by Battle_Run for each action of the round, with a delay
/// of 10 for the first. Resets the camera's view (gBattleBackgroundWork's
/// frames, flag and yaw, gBattleWork->brightness, the transform and
/// projection), builds gBattleWork->turn, and marks the action done
/// (request->unitId to BATTLE_UNIT_LIST_END) unless the party fled.
s32 BattleStaging_DispatchAction(struct BattleAction *request, s32 delay) {
  struct BattleBackgroundWork *view;
  struct BattleWork *battle;
  struct BattleCamera *camera;
  struct Unit *unit;
  u16 actingUnit[2];
  u16 unitIds[14];
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
  // An enemy settles its ability now, at its turn.
  if (unit->classId == CLASS_NONE)
    BattleAI_ChooseAction(request, 1);

  // The view: the camera eases over 60 frames, the background at full
  // brightness (1.0 in 16.16). The projection is the one
  // BattleCamera_SetView sets at 100 percent (focal length
  // 510 pixels), but with its far depth at 32767 pixels where that one stops at
  // 1020.
  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  view->frames = 60;
  battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  view->flag = 0;
  battle->brightness = Q16_ONE;
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  Transform_Reset();
  Transform_SetLookAt(camera->eye, camera->focus);
  Projection_Set(0x01fe0000, Iwram_RatioMulQ14(0x01fe0000, 0xc000), 0x7fff0000);
  if (delay != 0) {
    view->targetYaw = YAW_PARTY_TURN;
    WaitFrames(delay);
  }
  // Build the turn, with the actor highlighted, and stage it by its
  // outcome. A turn that failed has shown its message; TURN_REPORTED
  // waits for the player first.
  actingUnit[0] = request->unitId;
  actingUnit[1] = BATTLE_UNIT_LIST_END;
  BattleHighlight_SetModes(actingUnit, HIGHLIGHT_SELECTED);
  result = BattleTurn_Build(request, &battle->turn);
  if (result == 0) {
    switch (battle->turn.staging) {
    case STAGING_MESSAGES:
      BattleStaging_RunActorEntries(&battle->turn, 0);
      break;
    case STAGING_ATTACK:
      BattleStaging_RunAttack(&battle->turn, 0);
      break;
    case STAGING_SUMMON:
      BattleStaging_HideField(&battle->turn, STAGING_HIDE_ACTOR);
      break;
    case STAGING_FULL_SCREEN:
      BattleStaging_HideField(&battle->turn, 0);
      break;
    case STAGING_CAST:
      BattleStaging_RunPsynergyCast(&battle->turn, CAST_PSYNERGY);
      break;
    case STAGING_DJINNI:
      BattleStaging_RunPsynergyCast(&battle->turn, CAST_DJINNI);
      break;
    case STAGING_ENEMY_CAST:
      BattleStaging_RunPsynergyCast(&battle->turn, CAST_ENEMY);
      break;
    case STAGING_ITEM:
      BattleStaging_RunItem(&battle->turn, 0, request);
      break;
    case STAGING_FLEE:
      if (BattleStaging_RunFlee(&battle->turn) != 0)
        fled = 1;
      if (fled != 0)
        goto finish;
      break;
    }
  } else {
    if (result == TURN_REPORTED) {
      BattleMessage_WaitForAdvance();
      WaitFrames(3);
    }
    BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  }

  // The turn's end, then every unit back to its spot. A party that fled
  // skips this and the action's mark.
  BattleObject_DestroyAll();
  BattleUnit_ProcessTurnEnd(&battle->turn);
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

/// Fills an animation command from a turn: its kind and strength,
/// the actor and the targets still standing (or every target, for an animation
/// that revives), at least the first.
/// Called by each staging runner before its animation plays: RunUnitTransition,
/// RunAttack, RunPlainAttack, RunItem and RunPsynergyCast. Writes effect's
/// prelude (0), kind, variant, actor, actors, firstTarget, count and range
/// (1); the caller sets its side.
#ifdef UBFIX
void BattleStaging_BuildTargetList(struct BattleTurn *plan,
                                   struct BattleAnimArgs *effect) {
#else
s32 BattleStaging_BuildTargetList(struct BattleTurn *plan,
                                  struct BattleAnimArgs *effect) {
#endif
  s16 *outputTarget;
  u32 flags;
  s32 livingTargets;
  s32 targetIndex;
  u8 *inputTarget;

  livingTargets = 0;
  effect->prelude = 0;
  flags = plan->animation;
  effect->kind = flags & ANIM_ID_MASK;
  effect->variant = (flags & ANIM_VARIANT_MASK) >> ANIM_VARIANT_SHIFT;
  effect->user = plan->actorId;
  targetIndex = 0;
  if (targetIndex < plan->targetCount) {
    inputTarget = plan->targetIds;
    outputTarget = effect->targets;
    do {
      if (Unit_GetFar(*inputTarget)->hp != 0 ||
          (plan->animation & ANIM_ON_DOWNED)) {
        livingTargets++;
        *outputTarget++ = *inputTarget;
      }
      targetIndex++;
      inputTarget++;
    } while (targetIndex < plan->targetCount);
  }
  if (livingTargets == 0) {
    effect->targets[0] = plan->targetIds[0];
    livingTargets = 1;
  }
  effect->firstTarget = plan->targetIds[0];
  effect->targetCount = livingTargets;
  effect->range = 1;
}

/// Plays a unit's attempt to run. The flee roll was made as the round's
/// actions were gathered (BattleFlee_CheckSuccess); a failed one turns the
/// round into the enemies' (FIRST_STRIKE_ENEMIES), and that is
/// what this tests. So the party gets away unless the roll failed ("But
/// there's no escape!"), each member able to move running off in turn; an
/// enemy gets away 7 times in 10 and leaves the battle. Returns 1 when the
/// party got away.
/// Called by BattleStaging_DispatchAction for STAGING_FLEE. Turns the
/// camera to the party's view and holds gBattleBackgroundWork->busy while it
/// runs; an enemy that gets away is taken out of the lists and its object
/// released.
s32 BattleStaging_RunFlee(struct BattleTurn *plan) {
  struct BattleBackgroundWork *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  /* FAKEMATCH(temporary): completed cleared a halfword at a time, which keeps
   * partyMode a copy of its register (adds r2, r7, #0); as an s32 set to 0, 1
   * of 105 instructions differ in tbs-en. */
  union {
    s32 value;
    u16 halves[2];
  } completed;
  s32 partyMode;

  completed.halves[0] = 0;
  completed.halves[1] = 0;
  view->targetYaw = YAW_PARTY_TURN;
  view->busy = 1;
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);

  // The party: each member that can move runs, the last listed first, eight
  // frames apart.
  if (plan->actorId <= ALLY_MAX) {
    partyMode = completed.value;

    if (battle->firstStrike != FIRST_STRIKE_ENEMIES) {
      partyMode = 1;
    }

    if (!partyMode) {
      BattleMessage_ShowAndWaitFar(MsgNoEscape);
      BattleMessage_WaitForAdvance();
    } else {
      s16 unitIds[14];
      s32 index =
          BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)unitIds) - 1;

      while (index != -1) {
        struct Unit *unit = Unit_GetFar(unitIds[index]);

        if (unit->stun == 0 && unit->sleep == 0) {
          BattleMotion_SetupEscapeObject(unitIds[index]);
          WaitFrames(8);
        }
        index--;
      }
      WaitFrames(22);
      completed.value = 1;
    }
  } else if (((Random16() * 10) >> 16) <= 6) {
    // An enemy: a roll of 0 to 9, 7 in 10 away.
    s16 event[2];

    event[0] = plan->actorId;
    event[1] = BATTLE_UNIT_LIST_END;
    BattleMotion_SetupEscapeObject(event[0]);
    WaitFrames(8);
    BattleObject_RemoveFromLists(plan->actorId);
    BattleObject_MarkForRelease(plan->actorId);
  } else {
    BattleMessage_ShowAndWaitFar(MsgNoEscape);
    BattleMessage_WaitForAdvance();
  }

  view->busy = 0;
  return completed.value;
}

/// Plays a summon or a full-screen Psynergy. The camera turns to the target
/// side (or to the actor's, for a staged animation), the actor glows in its
/// element and the other side fades out; then, unless the cast failed
/// ("blocked" or too little PP), the target side's units are readied alone
/// and the animation plays during playback. Everyone fades back in.
/// Called by BattleStaging_DispatchAction for STAGING_SUMMON (with
/// STAGING_HIDE_ACTOR) and STAGING_FULL_SCREEN. Sets gBattleBackgroundWork's
/// yaw (and its frames, for an animation), REG_BLDCNT and REG_BLDALPHA for the
/// fades, and the units' colour modes; schedules BattlePlayback_Run around the
/// animation.
/// @param mode STAGING_HIDE_ACTOR hides the actor too (the summons)
s32 BattleStaging_HideField(struct BattleTurn *plan, s32 mode) {
  u16 units[14];
  struct BattleStagingWork work;
  u32 actor;
  u32 target;
  u32 count;
  s32 i;
  s32 n;
  u32 kept;
  struct Actor *object;
  // UB: indexing past scalar gBattleWork reaches another heap slot. Named
  // slots keep the native EN size (0x3ba), but reorder address setup.
#ifndef UBFIX
  void **slots = (void **)&gBattleWork;
#endif

  // The camera: a staged animation turns to the actor's side over 60 frames (an
  // enemy's to YAW_BEHIND_ENEMIES); otherwise the acting side's usual view.
  BattleStaging_BuildTargetList(plan, &work.effect);
  actor = plan->actorId;
  target = plan->targetIds[0];
  if (plan->animation & ANIM_SUMMON) {
    struct BattleBackgroundWork *view;
    s32 yaw;
#ifdef UBFIX
    view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    yaw = YAW_PARTY_TURN;
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
    yaw = YAW_ENEMY_TURN;
    if (actor <= ALLY_MAX)
      yaw = YAW_PARTY_TURN;
    if (view->targetYaw != yaw)
      view->targetYaw = yaw;
  }

  // The actor raises its hand (POSE_CAST) and glows; the units of the side
  // not targeted fade out over 16 frames (and the actor too, for
  // STAGING_HIDE_ACTOR).
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  PartyStatus_DrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);
  object = BattleObject_Get(actor)->object;
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
  Sound_PlaySongFar(SE_CAST);
  ElementGlow_Play(work.effect.user, plan->element, GLOW_STAR_FIELD, 0);
  if (mode & STAGING_HIDE_ACTOR)
    BattleHighlight_SetSpriteMode(actor, SPRITE_SEMI_TRANSPARENT);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = BLEND_ALPHA(16 - i, 16);
    WaitFrames(1);
  }
  // A failed cast: "blocked" (sealed) or too little PP, shown at once.
  if (plan->failure != TURN_FAILURE_NONE) {
    if (plan->failure == TURN_FAILURE_SEALED) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->actorId);
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgCastLacksPp);
    }
    BattlePlayback_RunAll();
    BattleBackground_RestorePalette();
  } else {
    // Only the target side's units stay on the field (with the actor, unless
    // STAGING_HIDE_ACTOR hides it); the list is then reused for the turn's
    // targets.
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
    // Save each target's layer colour modes (all but its last layer) in
    // work, beside the animation command. RunItem and RunPsynergyCast do the
    // same.
    // Nothing reads them back: every animation reads its command only
    // through struct BattleAnimArgs's named fields, which end before them,
    // and the colour modes are put back from BattleObject_GetPalette
    // (BattleAnim_SetupBlendedDisplay) or cleared afterwards.
    // XXX (no evidence): what they were kept for; no reader in this game's
    // battle code shows it.
    for (i = 0; i != work.effect.targetCount; i++) {
      struct Sprite *animation;
      u32 child;

      animation =
          GetMotionRecord(BattleObject_Get(work.effect.targets[i])->object, 0);
      for (child = 0; child != animation->count - 1; child++)
        work.savedAnimationParameters[i][child] =
            animation->entries[child]->colorMode;
    }
    // The side the animation plays on: the targets'.
    if (plan->animation & ANIM_SUMMON) {
      if (target > ALLY_MAX)
        work.effect.side = ANIM_SIDE_ENEMIES;
      else
        work.effect.side = ANIM_SIDE_PARTY;
    } else {
      if (plan->targetIds[0] <= ALLY_MAX)
        work.effect.side = ANIM_SIDE_PARTY;
      else
        work.effect.side = ANIM_SIDE_ENEMIES;
    }
    if (plan->animation & ANIM_OTHER_SIDE)
      work.effect.side ^= 1;
    // The animation runs to its end while playback, a task, plays
    // each target's outcome from the frame the blow lands.
    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (plan->animation & ANIM_SUMMON)
      BattleAnim_RunSummonEffectFar(&work.effect);
    else if (plan->animation & ANIM_BY_ID_RANGE)
      BattleAnim_DispatchByIdRangeFar(&work.effect);
    else
      BattleAnim_RunTableEffectFar(&work.effect);
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
    WaitFrames(1);
  }
  for (i = 0; i != count; i++)
    BattleHighlight_SetSpriteMode(units[i], SPRITE_OPAQUE);
  BattleCamera_SetView(0, 0, 0, 100);
  WaitFrames(1);
  return 0;
}

/// Clears the highlights, then applies the action to each target in turn and
/// plays its events; with no target, only waits for the player.
/// Called by BattleStaging_DispatchAction for STAGING_MESSAGES, an
/// action with no animation: each target's BattleAbility_ApplyToTarget is
/// played at once by BattlePlayback_RunAll.
#ifdef UBFIX
void BattleStaging_RunActorEntries(struct BattleTurn *plan, s32 unused) {
#else
s32 BattleStaging_RunActorEntries(struct BattleTurn *plan, s32 unused) {
#endif
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
  WaitFrames(1);
}

/// Plays an attack: the camera turns halfway to the attacker's view, the
/// attacker runs up to its first target over its swing's frames, and the
/// blow lands: a miss makes the target hop back; a hit plays its animation and
/// its playback, and a critical hit zooms in on the pair with only those two
/// shown. Both then return. A miss "nimbly dodges the blow".
/// Called by BattleStaging_DispatchAction for STAGING_ATTACK; mode is
/// unused. Sets gBattleBackgroundWork's yaw (and its flag for a critical hit's
/// zoom), the window registers (WIN0 and WIN1 to the battlefield, rows 16 to
/// 136, both turned on), and schedules BattlePlayback_Run around the
/// animation.
s32 BattleStaging_RunAttack(struct BattleTurn *plan, s32 mode) {
  struct BattleStagingWork work;
  struct Actor *object;
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
    struct BattleTurn *savedPlan = plan;

    /* FAKEMATCH(dead): plan cleared and restored under tests that emit
     * nothing, ending a CSE block so the first read of plan reloads it;
     * without them, 5 of 289 instructions differ in tbs-en. */
    if (view)
      plan = NULL;
    if (view)
      plan = savedPlan;
  }

  // The camera turns to halfway between YAW_PARTY_TURN and the
  // attacker's bearing from the centre, an enemy's taken from the other
  // side: 5 frames if it is there already, else 10.
  object = BattleObject_Get(plan->actorId)->object;
  z = object->z.fixed;
  x = object->x.fixed;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - 0x2000;
  if (plan->actorId > ALLY_MAX)
    adjusted = angle + 0x6000;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    WaitFrames(5);
  } else {
    view->targetYaw = facingAngle;
    WaitFrames(10);
  }

  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  BattleStaging_BuildTargetList(plan, &work.effect);
  // Severe Blow drops the status window's PP row while it plays.
  if (work.effect.kind == ANIM_ID_SEVERE_BLOW)
    PartyStatus_DrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);

  unit = Unit_GetFar(work.effect.user);
  Unit_GetFar(work.effect.targets[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  // The run up: as long as the attacker's POSE_LEAP, with its class's hop
  // height (16.16).
  animation = GetMotionRecord(BattleObject_Get(plan->actorId)->object, 0);
  frames =
      SpriteAnim_SumCommandLengthsFar(animation->entries[0]->animationId, 2, 1);
  BattleMotion_ApproachTarget(work.effect.user, work.effect.targets[0], frames,
                              BattleSprite_GetAttackHop(unit->id) << 16);
  Actor_SetAnimationSpeedFar(BattleObject_Get(work.effect.user)->object, 16);
  BattleObject_Get(work.effect.targets[0]);

  if ((u16)work.effect.targets[0] <= ALLY_MAX)
    work.effect.side = ANIM_SIDE_PARTY;
  else
    work.effect.side = ANIM_SIDE_ENEMIES;

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
    WaitFrames(10);
    BattleMotion_HopBack(work.effect.targets[0]);
    WaitFrames(2);
    WaitFrames(4);
    WaitFrames(10);
    BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->targetIds[0]);
    BattlePlayback_Push(PLAYBACK_MESSAGE, MsgNimblyDodgesBlow);
    BattlePlayback_RunAll();
    BattleMotion_ReturnToSpot(work.effect.targets[0]);
  } else {
    // A hit. A critical hit plays the animation's critical version, with only
    // the pair on the field and the camera zooming in on them (scale 100 to
    // 130) over the rest of the run up.
    zoomOnCritical = 0;
    work.effect.prelude = 0;
    if (plan->animation != 0)
      work.effect.prelude = 1;

    if (critical != 0) {
      work.effect.kind += ANIM_ID_CRITICAL_OFFSET;
      zoomOnCritical = 1;
      view->flag = 1;
      criticalPair[0] = work.effect.user;
      criticalPair[1] = work.effect.firstTarget;
      criticalPair[2] = BATTLE_UNIT_LIST_END;
      BattleObject_SpawnObjectsForList(criticalPair, 0);
    }

    // The animation starts 8 frames before the run up ends, at least one frame
    // after it began.
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
        WaitFrames(1);
      }
    }

    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (work.effect.kind != 0) {
      if (plan->animation & ANIM_BY_ID_RANGE)
        BattleAnim_DispatchByIdRangeFar(&work.effect);
      else
        BattleAnim_RunTableEffectFar(&work.effect);
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

/// Unused. Plays a plain attack run: the attacker runs up to its first target
/// and the blow's animation and playback play, or, with bit 1 of flags (a
/// miss), the target hops back; both then return. Nothing calls it.
/// BattleStaging_RunAttack without the hop height, the windows, the critical
/// zoom, the dodge message or playback; its playback is played at once after
/// the animation, and the camera's slower turn waits 20 frames.
UNUSED s32 BattleStaging_RunPlainAttack(struct BattleTurn *input, s32 flags) {
  struct BattleStagingWork work;
  struct BattleTurn *savedInput;
  struct Actor *object;
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
  object = BattleObject_Get(savedInput->actorId)->object;
  z = object->z.fixed;
  x = object->x.fixed;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - 0x2000;
  if (savedInput->actorId > ALLY_MAX)
    adjusted = angle + 0x6000;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    WaitFrames(5);
  } else {
    view->targetYaw = facingAngle;
    WaitFrames(20);
  }

  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  BattleStaging_BuildTargetList(savedInput, &work.effect);
  Unit_GetFar(work.effect.user);
  Unit_GetFar(savedInput->targetIds[0]);

  missed = flags & 2;
  animation = GetMotionRecord(BattleObject_Get(savedInput->actorId)->object, 0);
  frames =
      SpriteAnim_SumCommandLengthsFar(animation->entries[0]->animationId, 2, 1);
  BattleMotion_ApproachTarget(work.effect.user, savedInput->targetIds[0],
                              frames, 0);
  Actor_SetAnimationSpeedFar(BattleObject_Get(work.effect.user)->object, 16);
  BattleObject_Get(savedInput->targetIds[0]);

  if (savedInput->targetIds[0] <= ALLY_MAX)
    work.effect.side = ANIM_SIDE_PARTY;
  else
    work.effect.side = ANIM_SIDE_ENEMIES;
  if (missed != 0) {
    WaitFrames(10);
    BattleMotion_HopBack(savedInput->targetIds[0]);
    WaitFrames(2);
    WaitFrames(4);
    WaitFrames(10);
    BattleMotion_ReturnToSpot(savedInput->targetIds[0]);
  } else {
    BattleAnim_DispatchByIdRangeFar(&work.effect);
    BattlePlayback_RunAll();
    BattleMotion_ReturnToSpot(savedInput->targetIds[0]);
  }
  BattleMotion_ReturnToSpot(work.effect.user);
  return 0;
}

/// Plays an item's use: the camera turns to the user's side, the user raises
/// the item and, for an item with an animation, the screen dims while it plays
/// with its playback. Then the item is spent as its use type says; when its
/// slot empties, later item commands of the same member this round follow
/// their items to their new slots (or lose theirs, -1).
/// Called by BattleStaging_DispatchAction for STAGING_ITEM, with the
/// action's entry as selection. Sets gBattleBackgroundWork's yaw and, while an
/// animation plays, dims gBattleWork->brightness and BG palettes 6 to 13;
/// changes the user's inventory (Inventory_RemoveFar, Inventory_BreakFar, or
/// the Empty Bottle) and the parameters of gBattleWork->actions.
s32 BattleStaging_RunItem(struct BattleTurn *plan, s32 unused,
                          struct BattleAction *selection) {
  struct BattleStagingWork work;
  struct Actor *object;
  s32 targetOrFadeIndex;
  s32 inventoryEntry;
  struct Unit *unit;
  struct ItemDefinition *item;
  struct BattleBackgroundWork *view = gBattleBackgroundWork;
  s32 targetYaw;

  // The camera turns to the user's side; the user raises the item
  // (POSE_CAST).
  // The other runners test > ALLY_MAX; with > 4, Jenna, Sheba and Piers
  // take the enemies' view. In play only Isaac, Garet, Ivan and Mia fight,
  // so the two tests agree; only the debug party presets with Jenna
  // (debug_presets.c) let an item turn the camera the wrong way.
  // XXX (no evidence): whether > 4 is a slip; the play the game allows
  // behaves the same either way, so only the author's intent would say.
  if (plan->actorId > 4)
    targetYaw = YAW_ENEMY_TURN;
  else
    targetYaw = YAW_PARTY_TURN;
  if (view->targetYaw != targetYaw)
    view->targetYaw = targetYaw;
  BattleStaging_BuildTargetList(plan, &work.effect);
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  object = BattleObject_Get(work.effect.user)->object;
  Actor_SelectAnimationFar(object, POSE_CAST);
  Actor_SetAnimationSpeedFar(object, 16);
  if (plan->targetIds[0] <= ALLY_MAX)
    work.effect.side = ANIM_SIDE_PARTY;
  else
    work.effect.side = ANIM_SIDE_ENEMIES;

  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.effect.targetCount;
       targetOrFadeIndex++) {
    struct Sprite *animation = GetMotionRecord(
        BattleObject_Get(work.effect.targets[targetOrFadeIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetOrFadeIndex][entryIndex] =
          animation->entries[entryIndex]->colorMode;
  }

  // An item with an animation dims the background over 20 frames, to about two
  // thirds of its brightness (19 steps of 0x444 in 16.16), then plays it; one
  // with none waits a second.
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
      WaitFrames(1);
      targetOrFadeIndex++;
      fade += 0x444;
    } while (targetOrFadeIndex < 20);
    if (plan->animation & ANIM_BY_ID_RANGE)
      BattleAnim_DispatchByIdRangeFar(&work.effect);
    else
      BattleAnim_RunTableEffectFar(&work.effect);
  } else {
    WaitFrames(60);
  }
  BattlePlayback_WaitForReady();
  Actor_SelectAnimationFar(object, POSE_STANDING);
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

/// Draws every sprite of an object in a colour mode (enum SpriteColorMode):
/// colorMode on each sprite's first layer, SPRITE_COLOR_NORMAL on the others,
/// and every layer's frame set to 255 (none) so that it is drawn again.
/// Called by BattleMotion_FlashHit and BattlePlayback_Run (a hit's flashes
/// and a unit's redraw), with the flash's colour or the unit's own
/// (BattleObject_GetPalette).
#ifdef UBFIX
void BattleObject_SetColorMode(struct Actor *object, s32 colorMode) {
#else
s32 BattleObject_SetColorMode(struct Actor *object, s32 colorMode) {
#endif
  s32 recordIndex = 0;
  s32 childIndex;
  s32 entryCount;
  struct Sprite *record;
  struct SpriteAnim *child;
  struct SpriteAnim **children;
  /* FAKEMATCH(temporary): the mask in a variable and the frame read into
   * another, so 255 stays in a register across the calls and is or-ed in;
   * as child->frame |= 0xff, 10 of 46 instructions differ in tbs-en. */
  u8 lastFrameMask = 0xff;
  u8 previousFrame;

  while ((record = GetMotionRecord(object, recordIndex)) != NULL) {
    children = &record->entries[1];
    child = record->entries[0];
    previousFrame = child->frame;
    child->frame = previousFrame | lastFrameMask;
    entryCount = record->count;
    child->colorMode = colorMode;
    for (childIndex = 1; childIndex < entryCount; childIndex++) {
      child = *children++;
      child->colorMode = 0;
      child->frame = 0xff;
    }
    recordIndex++;
  }
}

/// Plays Psynergy, a Djinni or an enemy's cast. The camera turns toward
/// the targets from the caster's side, the caster raises its hand and glows
/// in its element (no glow for a Djinni); then the cast fails ("blocked" or
/// too little PP) or its animation plays with its playback, and the targets
/// return to their spots.
/// Called by BattleStaging_DispatchAction for STAGING_CAST
/// (CAST_PSYNERGY), STAGING_DJINNI (CAST_DJINNI) and STAGING_ENEMY_CAST
/// (CAST_ENEMY). Sets gBattleBackgroundWork's yaw (and its frames for a fixed
/// view), redraws the party status window without its PP row, and schedules
/// BattlePlayback_Run around the animation.
s32 BattleStaging_RunPsynergyCast(struct BattleTurn *plan, s32 mode) {
  struct BattleStagingWork work;
  struct BattleBackgroundWork *view = gBattleBackgroundWork;
  struct Actor *object;
  s32 isDjinni;
  s32 targetIndex;

  // The camera. ANIM_VIEW_ACROSS takes the other side's view (an enemy's
  // cast YAW_BEHIND_ENEMIES) over 60 frames. Otherwise it starts 0x1800 off the
  // caster's bearing (less for the party, more for an enemy) and goes three
  // quarters of the way to the caster's side's view; a cast on the caster's own
  // side takes 0x2400 instead (-0x2400 for an enemy). ANIM_VIEW_REVERSED
  // then overrides it with the opposite side's view.
  if (plan->animation & ANIM_VIEW_ACROSS) {
    view->targetYaw =
        plan->actorId <= ALLY_MAX ? YAW_ENEMY_TURN : YAW_BEHIND_ENEMIES;
    view->frames = 60;
  } else {
    struct Actor *actor = BattleObject_Get(plan->actorId)->object;
    s32 angle = (u16)ArcTan2(actor->x.fixed, actor->z.fixed);
    s32 targetYaw;

    if (plan->actorId <= ALLY_MAX)
      targetYaw = angle - 0x1800;
    else
      targetYaw = angle + 0x1800;
    targetYaw = (s16)targetYaw;
    targetYaw +=
        ((plan->actorId <= ALLY_MAX ? YAW_PARTY_TURN : YAW_ENEMY_TURN) -
         targetYaw) *
        3 / 4;
    if (!((plan->actorId <= ALLY_MAX) ^ (plan->targetIds[0] <= ALLY_MAX)))
      targetYaw = plan->actorId <= ALLY_MAX ? 0x2400 : -0x2400;
    if (view->targetYaw != targetYaw)
      view->targetYaw = targetYaw;
  }
  if (plan->animation & ANIM_VIEW_REVERSED) {
    view->targetYaw =
        plan->actorId <= ALLY_MAX ? YAW_ENEMY_TURN : YAW_PARTY_TURN;
    view->frames = 60;
  }

  // The caster raises its hand (POSE_CAST) and glows in its element (its
  // streaks for an enemy's cast); a Djinni does not glow, and its animation
  // plays its prelude instead.
  BattleStaging_BuildTargetList(plan, &work.effect);
  isDjinni = mode & CAST_DJINNI;
  if (isDjinni)
    work.effect.prelude = 1;
  BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
  PartyStatus_DrawFar(gBattleWork->partyStatus & ~PARTY_STATUS_SHOW_PP);
  object = BattleObject_Get(work.effect.user)->object;
  Actor_SelectAnimationFar(object, POSE_CAST);
  Actor_SetAnimationSpeedFar(object, 16);
  Sound_PlaySongFar(SE_CAST);
  if (mode & CAST_ENEMY)
    ElementGlow_Play(work.effect.user, plan->element, GLOW_STREAKS, 0);
  else if (!isDjinni)
    ElementGlow_Play(work.effect.user, plan->element, GLOW_STAR_FIELD, 0);
  if (plan->targetIds[0] <= ALLY_MAX)
    work.effect.side = ANIM_SIDE_PARTY;
  else
    work.effect.side = ANIM_SIDE_ENEMIES;

  for (targetIndex = 0; targetIndex != work.effect.targetCount; targetIndex++) {
    struct Sprite *animation = GetMotionRecord(
        BattleObject_Get(work.effect.targets[targetIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetIndex][entryIndex] =
          animation->entries[entryIndex]->colorMode;
  }

  // A failed cast is shown at once; otherwise the animation plays during
  // playback, and an ability with no animation only restores the palette.
  if (plan->failure != TURN_FAILURE_NONE) {
    if (plan->failure == TURN_FAILURE_SEALED) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, plan->actorId);
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(PLAYBACK_MESSAGE, MsgCastLacksPp);
    }
    BattlePlayback_RunAll();
    BattleBackground_RestorePalette();
  } else {
    Task_Add(BattlePlayback_Run, TASK_PASS_FRAME);
    if (work.effect.kind) {
      if (plan->animation & ANIM_BY_ID_RANGE)
        BattleAnim_DispatchByIdRangeFar(&work.effect);
      else
        BattleAnim_RunTableEffectFar(&work.effect);
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

/// Flashes unit id as it is hit: puts it in its damage pose and switches its
/// sprites between white and their own colours twice, two frames each, its
/// status panel highlighted.
/// Called for PLAYBACK_HIT by BattlePlayback_RunAll
/// (BattlePlayback_Run flashes over frames itself). Waits 8 frames and
/// redraws the party's status at the end.
void BattleMotion_FlashHit(s32 unitId, s32 unused0, s32 unused1) {
  u16 selection[2];
  u16 *selected;
  s32 listTerminator;
  s32 flashesRemaining;

  Unit_GetFar(unitId);
  Actor_SelectAnimationFar(BattleObject_Get(unitId)->object, POSE_DAMAGE);

  selected = selection;
  flashesRemaining = 1;
  do {
    listTerminator = BATTLE_UNIT_LIST_END;
    selected[1] = listTerminator;
    selected[0] = unitId;
    PartyStatus_HighlightMembersFar(selected);
    BattleObject_SetColorMode(BattleObject_Get(unitId)->object,
                              SPRITE_COLOR_WHITE);
    WaitFrames(2);

    selected[0] = unitId;
    PartyStatus_HighlightMembersFar(selected);
    BattleObject_SetColorMode(BattleObject_Get(unitId)->object,
                              BattleObject_GetPalette(unitId));
    WaitFrames(2);
    flashesRemaining--;
  } while (flashesRemaining >= 0);

  PartyStatus_DrawFar(gBattleWork->partyStatus);
}

/// Takes unit id out of the party's or the enemies' list and out of this
/// round's actions, and frees its name letter.
/// Called when a unit leaves the battle: by BattleStaging_RunFlee (an
/// enemy that gets away), BattleMotion_ShowDowned and PLAY_FELLED in
/// BattlePlayback_Run (a fallen enemy), and
/// BattleObject_DestroyTemporaryObject. Sets the unit's side to
/// BATTLE_SIDE_NONE and its list entry to BATTLE_UNIT_REMOVED, releases its
/// name letter (BattleEnemy_ReleaseNameLetter), and ends its actions this
/// round (unitId BATTLE_UNIT_LIST_END). A unit in neither list only has its
/// side changed.
void BattleObject_RemoveFromLists(s32 unitId) {
  struct BattleWork *battle;
  s32 partyIndex;
  u32 enemyOrActionIndex;
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
  // The scan is a label loop, the form its branches match with.
  /* FAKEMATCH(one-pass): the block keeps the party loop's first test from
   * being peeled ahead of it; without the block, 12 of 53 instructions differ
   * in tbs-en. */
  do {
    enemyOrActionIndex = 0;
  again:
    listedUnit = battle->enemyIds[enemyOrActionIndex];
    if (listedUnit == unitId) {
      battle->enemyIds[enemyOrActionIndex] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    enemyOrActionIndex++;
    if (listedUnit == BATTLE_UNIT_LIST_END)
      return;
    goto again;
  } while (0);

removed:
  // Free the letter that told the enemy apart from others of its class.
  BattleEnemy_ReleaseNameLetter(unitId);
  for (enemyOrActionIndex = 0; enemyOrActionIndex < BATTLE_ACTION_CAPACITY;
       enemyOrActionIndex++) {
    if (battle->actions[enemyOrActionIndex].unitId == unitId)
      battle->actions[enemyOrActionIndex].unitId = BATTLE_UNIT_LIST_END;
  }
}

/// Shows a downed unit: a party member lies down, an enemy keeps its damage
/// pose, turns grey, leaves the lists after four frames and dissolves a dot
/// at a time.
/// Called for PLAYBACK_DEFEAT by BattlePlayback_RunAll; PLAY_FELLED in
/// BattlePlayback_Run is its counterpart over frames. An enemy's object is
/// marked for release.
void BattleMotion_ShowDowned(s32 unitId) {
  void *records[4];
  struct Unit *unit;
  struct Sprite *record;
  struct SpriteAnim *entry;
  s32 recordCount;

  unit = Unit_GetFar(unitId);
  recordCount = 0;
  while ((record = GetMotionRecord(BattleObject_Get(unitId)->object,
                                   recordCount)) != 0) {
    if (unit->side != BATTLE_SIDE_ENEMY)
      Sprite_StartAnimFar(record, POSE_PARTY_HELD);
    else
      Sprite_StartAnimFar(record, POSE_ENEMY_DOWN);
    recordCount++;
  }

  if (unit->side == BATTLE_SIDE_ENEMY) {
    recordCount = 0;
    while ((record = GetMotionRecord(BattleObject_Get(unitId)->object,
                                     recordCount)) != 0) {
      entry = record->entries[0];
      records[recordCount] = record;
      entry->colorMode = SPRITE_COLOR_GREY;
      entry->frame = 0xff;
      recordCount++;
    }
    WaitFrames(4);
    BattleObject_RemoveFromLists(unitId);
    // The whole dissolve, over 32 frames.
    SpriteDissolve_RunManyFar((u8 **)records, recordCount);
    BattleObject_MarkForRelease(unitId);
  }
}
