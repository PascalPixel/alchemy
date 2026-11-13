/// @file
/// An action played: its targets gathered, the action dispatched, and the
/// actors' entries it runs.
// Part of battle.c.

#include "../battle_internal.h"
#include "module/battle/battle.h"
#include "gamepak/far_call/battle_effect.h"
#include "module/battle/presentation.h"

/// A party member's attack effect, staged by id range (0x4000): effect 1 for
/// Isaac, Garet, Felix, Jenna and Piers, 4 for Ivan, Mia and Sheba, any other
/// none. The Broken Seal writes the same choice inline in
/// BattleActionRun_Build.
s32 BattleUnit_GetPartyAttackEffect(s32 characterId) {
  s32 id = characterId;

  // characterId carries the effect from here on, in the register it came in.
  characterId = 0;
  switch (id) {
  case ISAAC:
  case GARET:
  case FELIX:
  case JENNA:
  case PIERS:
    characterId = 0x4001;
    break;
  case IVAN:
  case MIA:
  case SHEBA:
    characterId = 0x4004;
    break;
  }
  return characterId;
}

/// Called by Battle_Run at the start of every turn; does nothing, as in The
/// Broken Seal.
void Battle_TurnStartNoOp(void) {}

/* VIEW(battle): routine BattleAI_ChooseRandomTarget as s32
 * BattleAI_ChooseRandomTarget(); module/battle/presentation.h has s32
 * BattleAI_ChooseRandomTarget(s32 party). */
extern s32 Func_0811f4d4__battle() __asm__("BattleAI_ChooseRandomTarget");

void Func_0811d7e8(u16 *source, struct ActionTargets *targets) {
  u16 ids[10];
  s32 count;
  s32 i;
  s32 side = 2;
  s32 own;

  if (source[0] > 7)
    side = 1;
  count = BattleUnit_ListIds(side, ids);
  Iwram_ClearWordsEntry(targets, sizeof *targets);
  own = 0;
  if (source[0] <= 7)
    own = 1;
  own = Battle_GetUnitAtPlace(Func_0811f4d4__battle(own));
  targets->value50 = 2;
  targets->value54 = 14;
  {
    s32 half = source[3];
    u16 *value;

    value = &targets->value4a;
    targets->side = own;
    *value = half;
  }
  for (i = 0; i < count; i++) {
    targets->ids[i] = ids[i];
    targets->stateA[i] = 0;
    targets->stateB[i] = 1;
    targets->stateC[i] = 0;
    targets->stateD[i] = 1;
  }
  targets->count = count;
  targets->message = 403;
  targets->cue = 0x801a;
}

void Func_0811d89c(struct ActionTargets *targets) {
  u16 ids[10];
  s32 count;
  s32 i;
  s32 side = 1;

  if (targets->side > 7)
    side = 2;
  count = BattleUnit_ListIds(side, ids);
  for (i = 0; i < count; i++) {
    targets->ids[i] = ids[i];
    targets->stateA[i] = 0;
    targets->stateB[i] = 1;
    targets->stateC[i] = 0;
    targets->stateD[i] = 1;
  }
  targets->count = count;
  targets->message = 103;
  targets->cue = 0x10b5;
}

void Func_0811d914(struct ActionTargets *targets) {
  u16 ids[10];
  s32 count;
  s32 i;

  if (targets->side <= 7) {
    count = BattleParty_PrepareMembers(ids);
    count += BattleParty_PrepareReserveOwners(&ids[count]);
  } else {
    count = BattleParty_ListActorIds(2, ids);
  }
  for (i = 0; i < count; i++) {
    targets->ids[i] = ids[i];
    targets->stateA[i] = 0;
    targets->stateB[i] = 1;
    targets->stateC[i] = 0;
    targets->stateD[i] = 1;
  }
  {
    // The zero in a local of its own takes r3 and leaves r2 to the two
    // constants; as a plain 0, 18 of 69 instructions differ in tla-en.
    s32 zero = 0;

    targets->message = 298;
    targets->count = count;
    targets->cue = 0x120b6;
    targets->extra = zero;
  }
}

/// Sends every battler back to its spot.
void BattleParty_ReturnAllToSpots(void) {
  u16 ids[14];
  s32 count = BattleParty_ListActorIds(3, ids);
  s32 i;

  for (i = 0; i < count; i++)
    BattleMotion_ReturnToSpot(ids[i]);
}

/// Plays one action of the turn after delay frames, as The Broken Seal's
/// BattleStaging_DispatchAction does: an enemy picks its ability now, the
/// action is built and staged, and every unit returns to its spot. Returns 1
/// when the party got away, 0 otherwise, and -1 when the unit is already down.
s32 BattleStaging_DispatchAction(struct BattleActionRecord *request,
                                 s32 delay) {
  struct BattlePresentationTransition *view;
  struct DispatchBattleWork *battle;
  struct BattleCamera *camera;
  struct BattleUnit *unit;
  u16 actingUnit[2];
  s32 fled;
  s32 result;

  battle = (struct DispatchBattleWork *)Ram_HeapSlots->battleWork;
  fled = 0;
  if (request->unitId == 255)
    return 0;
  unit = Unit_Get(request->unitId);
  if (unit->hp == 0)
    return -1;
  battle->actorMode = 3;
  if (unit->classIndex == 0)
    BattleAI_ChooseAction(request, 1);

  view = Ram_HeapSlots->transitionWork;
  view->frames = 60;
  view->flag = 0;
  camera = Ram_HeapSlots->cameraWork;
  battle->brightness = 0x10000;
  Transform_Reset();
  Transform_SetLookAt__battle(camera->eye, camera->pos);
  {
    s32 param;
    s32 ratio;
    // Called through a local, as field/object/visual_attributes.c calls it:
    // the routine's address is then loaded ahead of the shift that finishes
    // param.
    s32 (*ratioMul)(s32, s32) = Iwram_RatioMulQ14;

    param = 255;
    param <<= 17;
    ratio = ratioMul(param, 192 << 8);
    param = 255;
    param <<= 17;
    Projection_Set(param, ratio, 0x7fff0000);
  }
  if (delay != 0) {
    view->targetYaw = 0x2000;
    wait(delay);
  }
  if (request->command == 10) {
    Func_0811d7e8((u16 *)request, (struct ActionTargets *)&battle->plan);
    /* FAKEMATCH(dead): an empty once-only block after the call, so the plan's
     * address is finished before request is copied to r0; without it, 2 of 421
     * instructions differ in tla-en. */
    do {
    } while (0);
    result = 0;
  } else {
    actingUnit[0] = request->unitId;
    actingUnit[1] = 255;
    BattleHighlight_SetModes(actingUnit, 1);
    result = BattleActionRun_Build(request, &battle->plan);
  }
  if (result == 0) {
    switch (battle->plan.outcome) {
    case 0:
      break;
    case 1:
      BattleStaging_RunActorEntries(&battle->plan, 0);
      break;
    case 2:
      BattleStaging_RunAttack(&battle->plan, 0);
      break;
    case 15:
      BattleStaging_RunAttack(&battle->plan, 2);
      break;
    case 5:
      BattleStaging_HideField(&battle->plan, 1);
      break;
    case 9:
      BattleStaging_HideField(&battle->plan, 0);
      break;
    case 14:
      BattleStaging_HideField(&battle->plan, 3);
      break;
    case 3:
    case 17:
      BattleStaging_RunPsynergyCast(&battle->plan, 0);
      break;
    case 6:
      BattleStaging_RunPsynergyCast(&battle->plan, 1);
      break;
    case 8:
    case 16:
      BattleStaging_RunPsynergyCast(&battle->plan, 2);
      break;
    case 4:
      BattleStaging_RunItem(&battle->plan, 0, request);
      break;
    case 10: {
      u8 swap = battle->plan.targetIds[0];

      battle->unk84e = 1;
      battle->plan.targetIds[0] = battle->plan.actorId2;
      battle->skipPlayback = 1;
      BattleStaging_RunPsynergyCast(&battle->plan, 1);
      battle->skipPlayback = 0;
      battle->plan.targetIds[0] = swap;
      swap = battle->plan.actorId;
      battle->plan.actorId = battle->plan.actorId2;
      battle->plan.actorId2 = swap;
      battle->plan.presentationFlags = BattleUnit_GetPartyAttackEffect(
          ((struct BattleUnit *)Unit_Get(battle->plan.actorId))->enemyId);
      if (((battle->plan.actorId ^ battle->plan.targetIds[0]) >> 7) == 0) {
        MessageArgument_PushFar(battle->plan.actorId, 1);
        if (battle->plan.actorId == battle->plan.targetIds[0])
          BattleMessage_ShowAndWaitFar((s32)&MsgStopHittingYourself);
        else
          BattleMessage_ShowAndWaitFar((s32)&MsgAttacksAlly);
      }
      BattleStaging_RunAttack(&battle->plan,
                              battle->plan.abilityId == 312 ? 32 : 0);
      swap = battle->plan.actorId;
      battle->plan.actorId = battle->plan.actorId2;
      battle->plan.actorId2 = swap;
      break;
    }
    case 11: {
      s32 kept;

      battle->skipPlayback = 1;
      BattleStaging_RunPsynergyCast(&battle->plan, 1);
      battle->skipPlayback = 0;
      BattleMessage_ShowAndWaitFar((s32)&MsgDjinnRecoverFaster);
      kept = DjinnRecovery_CountDownSide(battle->plan.targetIds[0] > 7);
      if (kept == 0)
        BattleMessage_ShowAndWaitFar((s32)(MsgDjinnRecoverFaster + 1));
      if (kept <= 1)
        BattleMessage_WaitForAdvance();
      break;
    }
    case 12:
      BattleStaging_HideField(&battle->plan, 1);
      if (BattleParty_ListLivingUnits(1, 0) == 0)
        break;
      if (BattleParty_ListLivingUnits(2, 0) == 0)
        break;
      Func_0811d89c((struct ActionTargets *)&battle->plan);
      BattleStaging_RunPsynergyCast(&battle->plan, 1);
      break;
    case 13:
      BattleStaging_HideField(&battle->plan, 1);
      Func_0811d914((struct ActionTargets *)&battle->plan);
      BattleStaging_RunPsynergyCast(&battle->plan, 1);
      break;
    case 7:
      if (BattleStaging_RunFlee(&battle->plan) != 0)
        fled = 1;
      if (fled != 0)
        goto finish;
      break;
    }
  } else {
    if (result == -1) {
      BattleMessage_WaitForAdvance();
      wait(3);
    }
    BattleHighlight_SetModes(0, 0);
  }

  BattleMotion_DestroyAllSlotObjects();
  BattleUnit_ProcessActionEnd(&battle->plan);
  BattleObject_Put();
  Enemy_Update();
  BattleParty_ReturnAllToSpots();
  request->unitId = 255;
finish:
  BattleBackground_SetMode(
      2, ((struct DispatchBattleWork *)Ram_HeapSlots->battleWork)->background,
      0);
  Sound_PlayFar(390);
  return fled;
}

void BattleStaging_BuildTargetList(struct BattlePlan *plan,
                                   struct BattlePresentationWork *effect) {
  s16 *outputTarget;
  u32 flags;
  s32 livingTargets;
  s32 targetIndex;
  u8 *inputTarget;

  livingTargets = 0;
  effect->flags = 0;
  flags = plan->presentationFlags;
  effect->kind = flags & 0xfff;
  effect->variant = (flags & 0x3000) >> 12;
  effect->actor = plan->actorId;
  targetIndex = 0;
  if (targetIndex < plan->targetCount) {
    inputTarget = plan->targetIds;
    outputTarget = effect->actors;
    do {
      if (BattleUnit_IsListed(*inputTarget) &&
          (((struct BattleUnit *)Unit_Get(*inputTarget))->hp != 0 ||
           (plan->presentationFlags & 0x10000))) {
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
  effect->initialValue = 1;
}

/// Plays a flee action, The Broken Seal's BattleStaging_RunFlee without its
/// last wait: the party runs unless encounterMode is 2, and an enemy gets away
/// 7 times in 10. Returns 1 when the party got away.
s32 BattleStaging_RunFlee(struct BattlePlan *plan) {
  s32 completed;
  struct EscapeTransition *presentation =
      (struct EscapeTransition *)Ram_HeapSlots->transitionWork;
  u32 aid = plan->actorId;
  struct BattleWork *scene = Ram_HeapSlots->battleWork;
  // Default: 0 until the party branch copies completed.
  s32 partyMode = 0;

  completed = 0;
  presentation->targetYaw = 0x2000;
  presentation->active = 1;
  BattleHighlight_SetModes(0, 0);

  if (aid <= 7) {
    partyMode = completed;
    if (scene->encounterMode != 2)
      partyMode = 1;

    if (!partyMode) {
      BattleMessage_ShowAndWaitFar((s32)MsgNoEscape);
      BattleMessage_WaitForAdvance();
    } else {
      s16 unitIds[14];
      s32 index = BattleParty_ListLivingUnits(1, (u16 *)unitIds) - 1;

      while (index != -1) {
        struct BattleUnit *unit = Unit_Get(unitIds[index]);

        if (unit->stun == 0 && unit->sleep == 0) {
          BattleMotion_SetupEscapeObject(unitIds[index]);
          wait(8);
        }
        index--;
      }
      completed = 1;
    }
  } else if (((Random16() * 10) >> 16) <= 6) {
    s16 event[2];

    event[0] = plan->actorId;
    event[1] = 0xff;
    BattleMotion_SetupEscapeObject(event[0]);
    wait(8);
    BattleObject_RemoveFromLists(plan->actorId);
    BattleObject_MarkForRelease(plan->actorId);
  } else {
    BattleMessage_ShowAndWaitFar((s32)MsgNoEscape);
    BattleMessage_WaitForAdvance();
  }

  presentation->active = 0;
  return completed;
}

/* VIEW(battle): routine BattlePlayback_WaitForReady as void
 * BattlePlayback_WaitForReady(void); module/battle/battle.h has s32
 * BattlePlayback_WaitForReady(void). */
extern void BattlePlayback_WaitForReady__battle(void) __asm__(
    "BattlePlayback_WaitForReady");

s32 BattleStaging_HideField(struct BattlePlan *plan, s32 mode) {
  u16 units[14];
  struct BattlePresentationWork work;
  u32 actor;
  u32 target;
  u32 count;
  u32 i;
  u32 kept;
  struct MotionObject *object;
  u16 *list;

  BattleStaging_BuildTargetList(plan, &work);
  actor = plan->actorId;
  target = plan->targetIds[0];
  if (plan->presentationFlags & 0x8000) {
    struct BattlePresentationTransition *view = Ram_HeapSlots->transitionWork;

    if (actor <= 7)
      view->targetYaw = 0x2000;
    else
      view->targetYaw = 0x5000;
    view->frames = 60;
  } else {
    struct BattlePresentationTransition *view = Ram_HeapSlots->transitionWork;
    s32 yaw;

    yaw = -0x2000;
    if (actor <= 7)
      yaw = 0x2000;
    if (view->targetYaw != yaw)
      view->targetYaw = yaw;
  }

  BattleHighlight_SetModes(0, 0);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  object = BattleObject_Get(actor)->object;
  REG_BLDCNT = 0x3f40;
  list = units;
  i = 0;
  count = BattleParty_ListActorIds(3, list);
  for (; i != count; i++) {
    if (units[i] == 254)
      continue;
    if ((mode & 2) || units[i] != actor) {
      u32 unit = units[i];

      if ((target <= 7) != (unit <= 7))
        BattleHighlight_SetSpriteMode(unit, 1);
    } else {
      Actor_SelectAnimationFar((struct Actor *)object,
                               ((u8 *)Ram_HeapSlots->battleWork)[0x869]);
    }
  }
  if (!(mode & 2)) {
    Sound_PlayFar(154);
    ElementGlow_Play(work.actor, plan->element, 0, 0);
  }
  if (mode & 1)
    BattleHighlight_SetSpriteMode(actor, 1);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = (16 - i) | 0x1000;
    wait(1);
  }

  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattlePlayback_Push(0, plan->actorId);
      BattlePlayback_Push(4, (u32)&MsgPsynergyBlocked);
    } else {
      BattlePlayback_Push(4, (u32)&MsgCastLacksPp);
    }
    BattlePlayback_RunAll();
    BattleBackground_RestorePalette();
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
    units[kept] = 255;
    BattleActor_SpawnObjectsForList((s16 *)units, 0);
    for (i = 0; i < plan->targetCount; i++)
      units[i] = plan->targetIds[i];
    units[i] = 255;
    for (i = 0; i != work.count; i++) {
      struct CastAnimation *animation =
          GetMotionRecord(BattleObject_Get(work.actors[i])->object, 0);
      u32 child;

      for (child = 0; child != animation->count - 1; child++)
        work.params[i][child] = animation->entries[child]->param;
    }
    if (plan->presentationFlags & 0x8000) {
      if (target <= 7)
        work.side = 1;
      else
        work.side = 0;
    } else {
      if (plan->targetIds[0] <= 7)
        work.side = 1;
      else
        work.side = 0;
    }
    if (plan->presentationFlags & 0x20000)
      work.side ^= 1;
    Task_Add(BattlePlayback_Run, TASK_PASS_VBLANK);
    if (plan->presentationFlags & 0x8000)
      BattleEffect_RunSceneEffectFar((s32 *)&work);
    else if (plan->presentationFlags & 0x4000)
      BattleEffect_DispatchByIdRangeFar(&work);
    else
      BattleEffect_RunFromTableFar(&work);
    BattlePlayback_WaitForReady__battle();
  }

  BattleObject_Put();
  count = BattleParty_ListActorIds(3, units);
  REG_BLDCNT = 0x3f40;
  for (i = 0; i != count; i++) {
    u32 unit;

    if (units[i] == 254)
      continue;
    if (!(mode & 2) && units[i] == actor && work.kind == 3)
      continue;
    unit = units[i];
    if ((target <= 7) != (unit <= 7))
      BattleHighlight_SetSpriteMode(unit, 1);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = i | 0x1000;
    wait(1);
  }
  for (i = 0; i != count; i++)
    BattleHighlight_SetSpriteMode(units[i], 0);
  BattleCamera_SetView(0, 0, 0, 100);
  wait(1);
  return 0;
}

/// Clears the highlights, applies each target's effect and runs the events.
/// With no targets, only waits for the message to advance.
/// plan: action and target information.
s32 BattleStaging_RunActorEntries(struct BattlePlan *plan, s32 unused) {
  s32 i;

  BattleHighlight_SetModes(0, 0);
  if (plan->targetCount == 0) {
    BattleMessage_WaitForAdvance();
  } else {
    for (i = 0; i < plan->targetCount; i++) {
      BattleTarget_Execute(plan, i);
      BattlePlayback_RunAll();
    }
  }
  wait(1);
}
