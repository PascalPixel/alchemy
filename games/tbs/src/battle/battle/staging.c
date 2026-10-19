/// @file
/// Part of battle.c, the battle object, compiled only through it.
/// An action played: its targets, and how an attack, a cast, an item, a
/// summon or an escape is staged.

#include "../battle_internal.h"

/// Plays one action of the round, after delay frames facing the party: an
/// enemy picks its ability now; the action is planned (its targets, hits
/// and damage) and shown the way its staging asks, then the turn's end
/// effects run and every unit returns to its spot. Returns 1 when the
/// party got away, 0 otherwise, and -1 when the actor is already down.
s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request,
                                      s32 delay) {
  struct BattlePresentationTransition *transition;
  struct BattleWork *battle;
  struct BattleCamera *camera;
  struct BattleUnit *unit;
  u16 actingUnit[2];
  u16 unitIds[14];
  s32 fled;
  s32 result;
  s32 unitCount;
  s32 unitIndex;

  fled = 0;
  if (request->unitId == BATTLE_UNIT_LIST_END)
    return 0;
  unit = UnitGetFar(request->unitId);
  if (unit->hp == 0)
    return -1;
  if (unit->classIndex == 0)
    BattleCommand_SelectAutomatic(request, 1);
  transition = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  transition->frames = 60;
  battle = WORK_SLOT(HEAP_SLOT_BATTLE);
  transition->flag = 0;
  battle->brightness = 0x10000;
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, camera->pos);
  Projection_Set(0x01fe0000, Iwram_RatioMulQ14(0x01fe0000, 0xc000), 0x7fff0000);
  if (delay != 0) {
    transition->targetYaw = YAW_PARTY_TURN;
    WaitFrames(delay);
  }
  actingUnit[0] = request->unitId;
  actingUnit[1] = BATTLE_UNIT_LIST_END;
  BattlePres_SetActorModes(actingUnit, 1);
  result = BattleCommand_BuildPlan(request, &battle->plan);
  if (result == 0) {
    switch (battle->plan.outcome) {
    case PLAN_SHOW_MESSAGES:
      BattlePres_RunActorEntries(&battle->plan, 0);
      break;
    case PLAN_SHOW_ATTACK:
      BattlePres_RunAttack(&battle->plan, 0);
      break;
    case PLAN_SHOW_SUMMON:
      BattlePresentation_RunUnitTransition(&battle->plan, 1);
      break;
    case PLAN_SHOW_FULL_SCREEN:
      BattlePresentation_RunUnitTransition(&battle->plan, 0);
      break;
    case PLAN_SHOW_CAST:
      BattlePres_RunPsynergyCast(&battle->plan, 0);
      break;
    case PLAN_SHOW_DJINNI:
      BattlePres_RunPsynergyCast(&battle->plan, 1);
      break;
    case PLAN_SHOW_ENEMY_CAST:
      BattlePres_RunPsynergyCast(&battle->plan, 2);
      break;
    case PLAN_SHOW_ITEM:
      BattlePres_RunItem(&battle->plan, 0, request);
      break;
    case PLAN_SHOW_FLEE:
      if (BattlePres_RunEscape(&battle->plan) != 0)
        fled = 1;
      if (fled != 0)
        goto finish;
      break;
    }
  } else {
    if (result == -1) {
      BattlePresentation_WaitForAdvance();
      WaitFrames(3);
    }
    BattlePres_SetActorModes(0, 0);
  }
  BattleMotion_DestroyAllSlotObjects();
  BattleUnit_ProcessTurnEnd(&battle->plan);
  BattleObjPut();
  unitCount = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
    ObjMotRst(unitIds[unitIndex]);
  // Done.
  request->unitId = BATTLE_UNIT_LIST_END;
finish:
  BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
  return fled;
}

/// Fills an effect command from a plan: its kind and strength,
/// the actor and the targets still standing (or every target, for an effect
/// that revives), at least the first.
s32 BattlePres_BuildTargetList(struct BattlePlan *plan,
                               struct BattleEffectArgument *effect) {
  // Callers ignore the result. As void, native English changes its return
  // pop/bx from r1 to r0; keep the existing unused integer return type.
  s16 *outputTarget;
  u32 flags;
  s32 livingTargets;
  s32 targetIndex;
  u8 *inputTarget;

  livingTargets = 0;
  effect->prelude = 0;
  flags = plan->presentationFlags;
  effect->kind = flags & STAGING_KIND_MASK;
  effect->variant = (flags & STAGING_VARIANT_MASK) >> STAGING_VARIANT_SHIFT;
  effect->actor = plan->actorId;
  targetIndex = 0;
  if (targetIndex < plan->targetCount) {
    inputTarget = plan->targetIds;
    outputTarget = effect->actors;
    do {
      if (UnitGetFar(*inputTarget)->hp != 0 ||
          (plan->presentationFlags & STAGING_ON_DOWNED)) {
        livingTargets++;
        *outputTarget++ = *inputTarget;
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

/// Plays a unit's attempt to run. The party gets away unless the enemies
/// struck first ("But there's no escape!"), each member able to move running
/// off in turn; an enemy gets away 7 times in 10 and leaves the battle.
/// Returns 1 when the party got away.
s32 BattlePres_RunEscape(struct BattlePlan *plan) {
  struct BattlePresentationTransition *presentation =
      WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleWork *scene = WORK_SLOT(HEAP_SLOT_BATTLE);
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
  presentation->targetYaw = YAW_PARTY_TURN;
  presentation->active = 1;
  BattlePres_SetActorModes(0, 0);

  if (plan->actorId <= 7) {
    partyMode = completed.value;

    if (scene->encounterMode != BATTLE_ENCOUNTER_ENEMIES_FIRST) {
      partyMode = 1;
    }

    if (!partyMode) {
      UiText_ShowMessageAndWaitCoreFar(MsgNoEscape);
      BattlePresentation_WaitForAdvance();
    } else {
      s16 unitIds[14];
      s32 index =
          BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, (u16 *)unitIds) - 1;

      while (index != -1) {
        struct BattleUnit *unit = UnitGetFar(unitIds[index]);

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
    s16 event[2];

    event[0] = plan->actorId;
    event[1] = BATTLE_UNIT_LIST_END;
    BattleMotion_SetupEscapeObject(event[0]);
    WaitFrames(8);
    BattleActor_RemoveFromLists(plan->actorId);
    BattleObj_MarkForRelease(plan->actorId);
  } else {
    UiText_ShowMessageAndWaitCoreFar(MsgNoEscape);
    BattlePresentation_WaitForAdvance();
  }

  presentation->active = 0;
  return completed.value;
}

/// Plays a summon or a full-screen Psynergy. The camera turns to the target
/// side (or to the actor's, for a staged scene), the actor glows in its
/// element and the other side fades out; then, unless the cast failed
/// ("blocked" or too little PP), the target side's units are readied alone
/// and the effect plays while the events run. Everyone fades back in.
/// mode : bit 0 hides the actor too (the summons)
s32 BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode) {
  u16 units[14];
  struct BattlePresentationWork work;
  u32 actor;
  u32 target;
  u32 count;
  s32 i;
  s32 n;
  u32 kept;
  struct MotionObject *object;
  // UB: indexing past scalar gBattleWork reaches another work slot. Named
  // slots keep the native EN size (0x3ba), but reorder address setup.
#ifndef UBFIX
  void **slots = (void **)&gBattleWork;
#endif

  BattlePres_BuildTargetList(plan, &work.effect);
  actor = plan->actorId;
  target = plan->targetIds[0];
  if (plan->presentationFlags & STAGING_SCENE) {
    struct BattlePresentationTransition *transition;
    s32 yaw;
#ifdef UBFIX
    transition = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    yaw = 0x2000;
    if (actor > 7)
      yaw = 0x5000;
    transition->targetYaw = yaw;
    transition->frames = 60;
  } else {
    struct BattlePresentationTransition *transition;
    s32 yaw;
#ifdef UBFIX
    transition = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
    transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
    yaw = -0x2000;
    if (actor <= 7)
      yaw = 0x2000;
    if (transition->targetYaw != yaw)
      transition->targetYaw = yaw;
  }
  BattlePres_SetActorModes(0, 0);
  PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
  object = BattleObjGet(actor)->object;
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
  for (i = 0; i != count; i++) {
    if (units[i] == BATTLE_UNIT_REMOVED)
      continue;
    if (units[i] != actor) {
      if ((target <= 7) != (units[i] <= 7))
        BattlePres_SetActorRecordMode(units[i], 1);
    } else {
      ObjectDispatch_SelectAnimationFar(object, 3);
    }
  }
  // The sound of a cast, as the caster raises its hand.
  AudioCommand_PlayFar(SE_CAST);
  BattleFx_PlayUnitElementEffect(work.effect.actor, plan->element, 0, 0);
  if (mode & 1)
    BattlePres_SetActorRecordMode(actor, 1);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = BLEND_ALPHA(16 - i, 16);
    WaitFrames(1);
  }
  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, plan->actorId);
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgCastLacksPp);
    }
    BattleEv_DispatchQueued();
    BattlePres_RestorePalette();
  } else {
    kept = 0;
    for (i = 0; i < count; i++) {
      u32 unit = units[i];
      if (unit == actor) {
        if (!(mode & 1))
          units[kept++] = actor;
      } else if ((target <= 7) == (unit <= 7)) {
        units[kept++] = unit;
      }
    }
    units[kept] = BATTLE_UNIT_LIST_END;
    BattleActor_SpawnObjectsForList(units, 0);
    for (n = 0; n < plan->targetCount; n++)
      units[n] = plan->targetIds[n];
    units[n] = BATTLE_UNIT_LIST_END;
    for (i = 0; i != work.effect.count; i++) {
      struct AnimationObject *animation;
      u32 child;

      animation =
          GetMotionRecord(BattleObjGet(work.effect.actors[i])->object, 0);
      for (child = 0; child != animation->count - 1; child++)
        work.savedAnimationParameters[i][child] =
            animation->entries[child]->param;
    }
    if (plan->presentationFlags & STAGING_SCENE) {
      if (target > 7)
        work.effect.side = 0;
      else
        work.effect.side = 1;
    } else {
      if (plan->targetIds[0] <= 7)
        work.effect.side = 1;
      else
        work.effect.side = 0;
    }
    if (plan->presentationFlags & STAGING_OTHER_SIDE)
      work.effect.side ^= 1;
    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
    if (plan->presentationFlags & STAGING_SCENE)
      BattleFx_RunSceneEffectFar(&work.effect);
    else if (plan->presentationFlags & STAGING_BY_ID_RANGE)
      BattleFx_DispatchByIdRangeFar(&work.effect);
    else
      BattleFx_RunTableEffectFar(&work.effect);
    BattleEventRuntime_WaitForReady();
  }
  BattleObjPut();
  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
  REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
  for (i = 0; i != count; i++) {
    if (units[i] == BATTLE_UNIT_REMOVED || units[i] == actor)
      continue;
    if ((target <= 7) != (units[i] <= 7))
      BattlePres_SetActorRecordMode(units[i], 1);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = BLEND_ALPHA(i, 16);
    WaitFrames(1);
  }
  for (i = 0; i != count; i++)
    BattlePres_SetActorRecordMode(units[i], 0);
  BattlePres_SetupTransitionScene(0, 0, 0, 100);
  WaitFrames(1);
  return 0;
}

/// Clears the highlights, then applies the action to each target in turn and
/// plays its events; with no target, only waits for the player.
/// plan : the action and its targets
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused) {
  s32 targetIndex;

  BattlePres_SetActorModes(0, 0);
  if (plan->targetCount == 0) {
    BattlePresentation_WaitForAdvance();
  } else {
    for (targetIndex = 0; targetIndex < plan->targetCount; targetIndex++) {
      BattleTargetExec(plan, targetIndex);
      BattleEv_DispatchQueued();
    }
  }
  WaitFrames(1);
}

/// Plays an attack: the camera turns halfway to the attacker's view, the
/// attacker runs up to its first target over its swing's frames, and the
/// blow lands: a miss makes the target hop back; a hit plays its effect and
/// the events, and a critical hit zooms in on the pair with only those two
/// shown. Both then return. A miss "nimbly dodges the blow".
s32 BattlePres_RunAttack(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  struct MotionObject *object;
  struct AnimationObject *animation;
  struct BattleUnit *unit;
  s16 criticalPair[3];
  s32 critical;
  struct BattlePresentationTransition *transition;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 x;
  s32 z;
  s32 frames;
  s32 missed;
  s32 index;
  s32 zoomOnCritical;

  transition = gTransitionWork;
  {
    struct BattlePlan *savedPlan = plan;

    /* FAKEMATCH(dead): plan cleared and restored under tests that emit
     * nothing, ending a CSE block so the first read of plan reloads it;
     * without them, 5 of 289 instructions differ in tbs-en. */
    if (transition)
      plan = NULL;
    if (transition)
      plan = savedPlan;
  }
  object = BattleObjGet(plan->actorId)->object;
  z = object->z;
  x = object->x;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - 0x2000;
  if (plan->actorId > 7)
    adjusted = angle + 0x6000;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

  if (transition->targetYaw == facingAngle) {
    transition->targetYaw = facingAngle;
    WaitFrames(5);
  } else {
    transition->targetYaw = facingAngle;
    WaitFrames(10);
  }

  BattlePres_SetActorModes(0, 0);
  BattlePres_BuildTargetList(plan, &work.effect);
  if (work.effect.kind == 0x87)
    PartyStatusDrawFar(gBattleWork->partyStatus & ~1);

  unit = UnitGetFar(work.effect.actor);
  UnitGetFar(work.effect.actors[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  animation = GetMotionRecord(BattleObjGet(plan->actorId)->object, 0);
  frames = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->animId,
                                                 2, 1);
  BattleMotion_ApproachTarget(work.effect.actor, work.effect.actors[0], frames,
                              BattleClass_GetAttackHop(unit->classId) << 16);
  ObjectDispatch_SetAnimationSpeedFar(
      (struct DispatchObject *)BattleObjGet(work.effect.actor)->object, 16);
  BattleObjGet(work.effect.actors[0]);

  if ((u16)work.effect.actors[0] <= 7)
    work.effect.side = 1;
  else
    work.effect.side = 0;

  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = BATTLE_WININ;
  REG_WINOUT = BATTLE_WINOUT;
  REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;

  if (missed != 0) {
    WaitFrames(10);
    BattleMotion_ResetObjectAtScaledAnchor(work.effect.actors[0]);
    WaitFrames(2);
    WaitFrames(4);
    WaitFrames(10);
    BattleEvPush(BATTLE_EVENT_UNIT_NAME, plan->targetIds[0]);
    BattleEvPush(BATTLE_EVENT_MESSAGE, MsgNimblyDodgesBlow);
    BattleEv_DispatchQueued();
    ObjMotRst(work.effect.actors[0]);
  } else {
    zoomOnCritical = 0;
    work.effect.prelude = 0;
    if (plan->presentationFlags != 0)
      work.effect.prelude = 1;

    if (critical != 0) {
      work.effect.kind += 200;
      zoomOnCritical = 1;
      transition->flag = 1;
      criticalPair[0] = work.effect.actor;
      criticalPair[1] = work.effect.firstTarget;
      criticalPair[2] = BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForList(criticalPair, 0);
    }

    frames -= 8;
    if (frames <= 0)
      frames = 1;
    {
      s32 first;
      s32 second;

      for (index = 0; index != frames; index++) {
        if (zoomOnCritical != 0) {
          first = work.effect.actor;
          second = work.effect.firstTarget;
          BattlePres_SetupTransitionAtPairMidpoint(first, second,
                                                   index * 30 / frames + 100);
        }
        WaitFrames(1);
      }
    }

    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
    if (work.effect.kind != 0) {
      if (plan->presentationFlags & STAGING_BY_ID_RANGE)
        BattleFx_DispatchByIdRangeFar(&work.effect);
      else
        BattleFx_RunTableEffectFar(&work.effect);
    }
    BattleEventRuntime_WaitForReady();
    if (critical != 0) {
      transition->flag = 0;
      BattleParty_ListAllUnitsAndSubmit();
      BattlePres_SetupTransitionScene(0, 0, 0, 100);
    }
    ObjMotRst(work.effect.actors[0]);
  }
  ObjMotRst(work.effect.actor);
  return 0;
}

/// Unused. Plays a plain attack run: the attacker runs up to its first target
/// and the blow's effect and events play, or, with bit 1 of flags (a miss), the
/// target hops back; both then return.
UNUSED s32 BattlePres_RunPlainAttack(struct BattlePlan *input, s32 flags) {
  struct BattlePresentationWork work;
  struct BattlePlan *savedInput;
  struct MotionObject *object;
  struct AnimationObject *animation;
  struct BattlePresentationTransition *transition;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 x;
  s32 z;
  s32 frames;
  s32 missed;

  transition = gTransitionWork;
  savedInput = input;
  object = BattleObjGet(savedInput->actorId)->object;
  z = object->z;
  x = object->x;
  angle = (u16)ArcTan2(x, z);
  adjusted = angle - 0x2000;
  if (savedInput->actorId > 7)
    adjusted = angle + 0x6000;
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

  if (transition->targetYaw == facingAngle) {
    transition->targetYaw = facingAngle;
    WaitFrames(5);
  } else {
    transition->targetYaw = facingAngle;
    WaitFrames(20);
  }

  BattlePres_SetActorModes(0, 0);
  BattlePres_BuildTargetList(savedInput, &work.effect);
  UnitGetFar(work.effect.actor);
  UnitGetFar(savedInput->targetIds[0]);

  missed = flags & 2;
  animation = GetMotionRecord(BattleObjGet(savedInput->actorId)->object, 0);
  frames = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->animId,
                                                 2, 1);
  BattleMotion_ApproachTarget(work.effect.actor, savedInput->targetIds[0],
                              frames, 0);
  ObjectDispatch_SetAnimationSpeedFar(
      (struct DispatchObject *)BattleObjGet(work.effect.actor)->object, 16);
  BattleObjGet(savedInput->targetIds[0]);

  if (savedInput->targetIds[0] <= 7)
    work.effect.side = 1;
  else
    work.effect.side = 0;
  if (missed != 0) {
    WaitFrames(10);
    BattleMotion_ResetObjectAtScaledAnchor(savedInput->targetIds[0]);
    WaitFrames(2);
    WaitFrames(4);
    WaitFrames(10);
    ObjMotRst(savedInput->targetIds[0]);
  } else {
    BattleFx_DispatchByIdRangeFar(&work.effect);
    BattleEv_DispatchQueued();
    ObjMotRst(savedInput->targetIds[0]);
  }
  ObjMotRst(work.effect.actor);
  return 0;
}

/// Inventory_RemoveFar's result when the last one left its slot, and the
/// slots after it moved up.
#define ITEM_REMOVED_SLOT_GONE 2

/// Plays an item's use: the camera turns to the user's side, the user raises
/// the item and, for an item with an effect, the screen dims while it plays
/// with the events. Then the item is spent as its use type says; when its
/// slot empties, later item commands of the same member this round follow
/// their items to their new slots (or lose theirs, -1).
s32 BattlePres_RunItem(struct BattlePlan *plan, s32 unused,
                       struct BattleActionRecord *selection) {
  struct BattlePresentationWork work;
  struct MotionObject *object;
  s32 targetOrFadeIndex;
  s32 inventoryEntry;
  struct BattleUnit *unit;
  struct ItemDefinition *itemDefinition;
  struct BattlePresentationTransition *transition = gTransitionWork;
  s32 targetYaw;

  if (plan->actorId > 4)
    targetYaw = YAW_ENEMY_TURN;
  else
    targetYaw = YAW_PARTY_TURN;
  if (transition->targetYaw != targetYaw)
    transition->targetYaw = targetYaw;
  BattlePres_BuildTargetList(plan, &work.effect);
  BattlePres_SetActorModes(0, 0);
  object = BattleObjGet(work.effect.actor)->object;
  ObjectDispatch_SelectAnimationFar(object, 3);
  ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, 16);
  if (plan->targetIds[0] <= 7)
    work.effect.side = 1;
  else
    work.effect.side = 0;

  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.effect.count;
       targetOrFadeIndex++) {
    struct AnimationObject *animation = GetMotionRecord(
        BattleObjGet(work.effect.actors[targetOrFadeIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetOrFadeIndex][entryIndex] =
          animation->entries[entryIndex]->param;
  }

  Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
  if (work.effect.kind != 0) {
    s32 fade;

    targetOrFadeIndex = 0;
    fade = 0;
    do {
      struct BattleWork *battle = gBattleWork;

      if (targetOrFadeIndex < 20) {
        u16 *palette = battle->palette;
        s32 value = 0x10000 - fade;

        battle->brightness = value;
        Graphics_ScaleRgb555Clamped(palette, (u16 *)(BG_PLTT + 6 * 16), value,
                                    128);
      }
      WaitFrames(1);
      targetOrFadeIndex++;
      fade += 0x444;
    } while (targetOrFadeIndex < 20);
    if (plan->presentationFlags & STAGING_BY_ID_RANGE)
      BattleFx_DispatchByIdRangeFar(&work.effect);
    else
      BattleFx_RunTableEffectFar(&work.effect);
  } else {
    WaitFrames(60);
  }
  BattleEventRuntime_WaitForReady();
  ObjectDispatch_SelectAnimationFar(object, 1);
  for (targetOrFadeIndex = 0; targetOrFadeIndex != work.effect.count;
       targetOrFadeIndex++)
    ObjMotRst(work.effect.actors[targetOrFadeIndex]);

  unit = UnitGetFar(selection->unitId);
  inventoryEntry = unit->inventory[selection->parameter];
  itemDefinition = Item_GetDirectFar(inventoryEntry);
  if (itemDefinition->useType == ITEM_USE_CONSUMED) {
    s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
    s32 index = selection->parameter;
    if (result == ITEM_REMOVED_SLOT_GONE) {
      struct BattleWork *battle = gBattleWork;
      u32 row;

      for (row = 0; row < BATTLE_ACTION_CAPACITY; row++) {
        if (battle->actions[row].command == CMD_ITEM &&
            battle->actions[row].unitId == selection->unitId) {
          if (battle->actions[row].parameter == index)
            battle->actions[row].parameter = -1;
          else if (battle->actions[row].parameter > index)
            battle->actions[row].parameter--;
        }
      }
    }
  } else if (itemDefinition->useType == ITEM_USE_MAY_BREAK) {
    if ((BattleRandomFar() & 7) == 0) {
      BattleEvPush(BATTLE_EVENT_ITEM_NAME,
                   unit->inventory[selection->parameter]);
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgUsedItemBroke);
      Inventory_BreakFar(selection->unitId, selection->parameter);
      BattleEv_DispatchQueued();
    }
  } else if (itemDefinition->useType == ITEM_USE_EMPTIES) {
    if ((inventoryEntry & ITEM_ID_MASK) == ITEM_HERMES_WATER)
      inventoryEntry = ITEM_EMPTY_BOTTLE;
    unit->inventory[selection->parameter] = inventoryEntry;
  }
  return 0;
}

/// Gives every sprite of an object palette value on its first entry (0 on
/// the others) and restarts each entry's frame.
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object,
                                      s32 palette) {
  s32 recordIndex = 0;
  s32 childIndex;
  s32 entryCount;
  struct AnimationObject *record;
  struct AnimationEntry *child;
  struct AnimationEntry **children;
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
    child->param = palette;
    for (childIndex = 1; childIndex < entryCount; childIndex++) {
      child = *children++;
      child->param = 0;
      child->frame = 0xff;
    }
    recordIndex++;
  }
}

/// Plays Psynergy, a Djinni or an enemy's cast. The camera turns toward
/// the targets from the caster's side, the caster raises its hand and glows
/// in its element (no glow for a Djinni); then the cast fails ("blocked" or
/// too little PP) or its effect plays with the events, and the targets
/// return to their spots.
/// mode : 1 a Djinni, 2 an enemy's cast, 0 any other
s32 BattlePres_RunPsynergyCast(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition = gTransitionWork;
  struct MotionObject *object;
  s32 isDjinni;
  s32 targetIndex;

  if (plan->presentationFlags & STAGING_VIEW_ACROSS) {
    transition->targetYaw = plan->actorId <= 7 ? -0x2000 : 0x5000;
    transition->frames = 60;
  } else {
    struct MotionObject *actor = BattleObjGet(plan->actorId)->object;
    s32 angle = (u16)ArcTan2(actor->x, actor->z);
    s32 targetYaw;

    if (plan->actorId <= 7)
      targetYaw = angle - 0x1800;
    else
      targetYaw = angle + 0x1800;
    targetYaw = (s16)targetYaw;
    targetYaw += ((plan->actorId <= 7 ? 0x2000 : -0x2000) - targetYaw) * 3 / 4;
    if (!((plan->actorId <= 7) ^ (plan->targetIds[0] <= 7)))
      targetYaw = plan->actorId <= 7 ? 0x2400 : -0x2400;
    if (transition->targetYaw != targetYaw)
      transition->targetYaw = targetYaw;
  }
  if (plan->presentationFlags & STAGING_VIEW_REVERSED) {
    transition->targetYaw = plan->actorId <= 7 ? -0x2000 : 0x2000;
    transition->frames = 60;
  }

  BattlePres_BuildTargetList(plan, &work.effect);
  isDjinni = mode & 1;
  if (isDjinni)
    work.effect.prelude = 1;
  BattlePres_SetActorModes(0, 0);
  PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
  object = BattleObjGet(work.effect.actor)->object;
  ObjectDispatch_SelectAnimationFar(object, 3);
  ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, 16);
  // The sound of a cast, as the caster raises its hand.
  AudioCommand_PlayFar(SE_CAST);
  if (mode & 2)
    BattleFx_PlayUnitElementEffect(work.effect.actor, plan->element, 1, 0);
  else if (!isDjinni)
    BattleFx_PlayUnitElementEffect(work.effect.actor, plan->element, 0, 0);
  if (plan->targetIds[0] <= 7)
    work.effect.side = 1;
  else
    work.effect.side = 0;

  for (targetIndex = 0; targetIndex != work.effect.count; targetIndex++) {
    struct AnimationObject *animation = GetMotionRecord(
        BattleObjGet(work.effect.actors[targetIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.savedAnimationParameters[targetIndex][entryIndex] =
          animation->entries[entryIndex]->param;
  }
  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, plan->actorId);
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgPsynergyBlocked);
    } else {
      BattleEvPush(BATTLE_EVENT_MESSAGE, MsgCastLacksPp);
    }
    BattleEv_DispatchQueued();
    BattlePres_RestorePalette();
  } else {
    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
    if (work.effect.kind) {
      if (plan->presentationFlags & STAGING_BY_ID_RANGE)
        BattleFx_DispatchByIdRangeFar(&work.effect);
      else
        BattleFx_RunTableEffectFar(&work.effect);
    } else {
      BattlePres_RestorePalette();
    }
    BattleEventRuntime_WaitForReady();
    ObjectDispatch_SelectAnimationFar(object, 1);
    for (targetIndex = 0; targetIndex != work.effect.count; targetIndex++)
      ObjMotRst(work.effect.actors[targetIndex]);
  }
  return 0;
}

/// Flashes unit id as it is hit: puts it in pose 5 and switches its sprites
/// between palette 7 and its own twice, two frames each, its status panel
/// highlighted.
void BattleMotion_FlashHit(s32 unitId, s32 unused0, s32 unused1) {
  u16 selection[2];
  u16 *selected;
  s32 listTerminator;
  s32 flashesRemaining;

  UnitGetFar(unitId);
  ObjectDispatch_SelectAnimationFar(BattleObjGet(unitId)->object, 5);

  selected = selection;
  flashesRemaining = 1;
  do {
    listTerminator = BATTLE_UNIT_LIST_END;
    selected[1] = listTerminator;
    selected[0] = unitId;
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObjGet(unitId)->object, 7);
    WaitFrames(2);

    selected[0] = unitId;
    BattleLayout_HighlightPartyPanelsFar(selected);
    BattleMotion_SetRecordChildValues(BattleObjGet(unitId)->object,
                                      BattleMotion_GetSlotPalette(unitId));
    WaitFrames(2);
    flashesRemaining--;
  } while (flashesRemaining >= 0);

  PartyStatusDrawFar(gBattleWork->partyStatus);
}

/// Takes unit id out of the party's or the enemies' list and out of this
/// round's actions, and frees its place in the enemy count.
void BattleActor_RemoveFromLists(s32 unitId) {
  struct BattleWork *battle;
  s32 partyIndex;
  u32 enemyOrActionIndex;
  s32 listedUnit;

  // The party's list.
  battle = gBattleWork;
  UnitGetFar(unitId)->side = BATTLE_UNIT_ABSENT;
  for (partyIndex = 0;; partyIndex++) {
    if (battle->partyUnits[partyIndex] == unitId) {
      battle->partyUnits[partyIndex] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (battle->partyUnits[partyIndex] == BATTLE_UNIT_LIST_END)
      break;
  }

  // The enemies' list.
  /* FAKEMATCH(one-pass): the enemy scan in a one-pass block, which keeps the
   * party loop's shape (its first test not peeled ahead of it); without the
   * block, 12 of 53 instructions differ in tbs-en. The scan itself is a label
   * loop, the form its branches match with. */
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

/// Shows a downed unit: a party member's sprite in animation 4, an enemy's in
/// 5; an enemy then leaves the lists after four frames and its sprite dissolves
/// a dot at a time.
void BattleMotion_ShowDowned(s32 unitId) {
  void *records[4];
  struct BattleUnit *unit;
  struct AnimationObject *record;
  struct AnimationEntry *entry;
  s32 recordCount;

  unit = UnitGetFar(unitId);
  recordCount = 0;
  while ((record = GetMotionRecord(BattleObjGet(unitId)->object,
                                   recordCount)) != 0) {
    if (unit->side != BATTLE_UNIT_ENEMY)
      AnimationObjects_SelectAnimationFar(record, 4);
    else
      AnimationObjects_SelectAnimationFar(record, 5);
    recordCount++;
  }

  if (unit->side == BATTLE_UNIT_ENEMY) {
    recordCount = 0;
    while ((record = GetMotionRecord(BattleObjGet(unitId)->object,
                                     recordCount)) != 0) {
      entry = record->entries[0];
      records[recordCount] = record;
      entry->param = 6;
      entry->frame = 0xff;
      recordCount++;
    }
    WaitFrames(4);
    BattleActor_RemoveFromLists(unitId);
    // A dot at a time.
    Map_RenderAllAnimatedTileFramesFar(records, recordCount);
    BattleObj_MarkForRelease(unitId);
  }
}
