/// @file
/// An action played: its targets gathered, the action dispatched, and the
/// actors' entries it runs.
// Part of battle.c.

#include "../battle_internal.h"

s32 Func_0811d79c(s32 kind) {
  s32 value = kind;

  kind = 0;
  switch (value) {
  case 0:
  case 1:
  case 4:
  case 5:
  case 7:
    kind = 0x4001;
    break;
  case 2:
  case 3:
  case 6:
    kind = 0x4004;
    break;
  }
  return kind;
}

void Func_0811d7e4(void) {}

struct ActionTargets {
  u8 side;
  u8 count;
  u8 unk02;
  u8 ids[14];
  u8 stateA[14];
  u8 stateB[14];
  u8 stateC[14];
  u8 stateD[14];
  u16 value4a;
  s32 message;
  s32 value50;
  s32 value54;
  s32 cue;
  s32 unk5c;
  s32 extra;
  s32 unk64;
};

s32 Func_0811f4d4();

void Func_0811d7e8(u16 *source, struct ActionTargets *targets) {
  u16 ids[10];
  s32 count;
  s32 i;
  s32 side = 2;
  s32 own;

  if (source[0] > 7)
    side = 1;
  count = Func_0811a39c(side, ids);
  Iwram_ClearWordsEntry(targets, sizeof *targets);
  own = 0;
  if (source[0] <= 7)
    own = 1;
  own = Battle_GetTaggedSlotValue(Func_0811f4d4(own));
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
  count = Func_0811a39c(side, ids);
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
    count = BattleParty_PrepareActiveOwners(ids);
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

void Func_0811d9a4(void) {
  u16 ids[14];
  s32 count = BattleParty_ListActorIds(3, ids);
  s32 i;

  for (i = 0; i < count; i++)
    ObjMotRst(ids[i]);
}

struct DispatchBattleWork {
  u8 unk000[0x66c];
  s32 brightness;
  u16 background;
  u8 unk672[0x67c - 0x672];
  struct BattlePlan plan;
  s32 skipPlayback;
  u8 unk6e4[0x84e - 0x6e4];
  u16 unk84e;
  u8 unk850[0x869 - 0x850];
  u8 actorMode;
};

s32 Func_0812381c(struct BattleActionRecord *request, struct BattlePlan *plan);
s32 Func_0811ea0c(struct BattlePlan *plan, s32 mode);
s32 Func_0811df70(struct BattlePlan *plan, s32 mode);
s32 Func_0811f088(struct BattlePlan *plan, s32 mode);
s32 Func_0811edb0(struct BattlePlan *plan, s32 unused,
                  struct BattleActionRecord *selection);
s32 Func_0811de78(struct BattlePlan *plan);
s32 Func_08124cc4(s32 party);
void Func_0812561c(struct BattlePlan *plan);
void Func_0812628c(s32 mode, s32 background, s32 value);

s32 Func_0811d9cc(struct BattleActionRecord *request, s32 delay) {
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
    Func_08122514(request, 1);

  view = Ram_HeapSlots->transitionWork;
  view->frames = 60;
  view->flag = 0;
  camera = Ram_HeapSlots->cameraWork;
  battle->brightness = 0x10000;
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, camera->pos);
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
    result = Func_0812381c(request, &battle->plan);
  }
  if (result == 0) {
    switch (battle->plan.outcome) {
    case 0:
      break;
    case 1:
      BattleStaging_RunActorEntries(&battle->plan, 0);
      break;
    case 2:
      Func_0811ea0c(&battle->plan, 0);
      break;
    case 15:
      Func_0811ea0c(&battle->plan, 2);
      break;
    case 5:
      Func_0811df70(&battle->plan, 1);
      break;
    case 9:
      Func_0811df70(&battle->plan, 0);
      break;
    case 14:
      Func_0811df70(&battle->plan, 3);
      break;
    case 3:
    case 17:
      Func_0811f088(&battle->plan, 0);
      break;
    case 6:
      Func_0811f088(&battle->plan, 1);
      break;
    case 8:
    case 16:
      Func_0811f088(&battle->plan, 2);
      break;
    case 4:
      Func_0811edb0(&battle->plan, 0, request);
      break;
    case 10: {
      u8 swap = battle->plan.targetIds[0];

      battle->unk84e = 1;
      battle->plan.targetIds[0] = battle->plan.actorId2;
      battle->skipPlayback = 1;
      Func_0811f088(&battle->plan, 1);
      battle->skipPlayback = 0;
      battle->plan.targetIds[0] = swap;
      swap = battle->plan.actorId;
      battle->plan.actorId = battle->plan.actorId2;
      battle->plan.actorId2 = swap;
      battle->plan.presentationFlags = Func_0811d79c(
          ((struct BattleUnit *)Unit_Get(battle->plan.actorId))->enemyId);
      if (((battle->plan.actorId ^ battle->plan.targetIds[0]) >> 7) == 0) {
        MessageArgument_PushFar(battle->plan.actorId, 1);
        if (battle->plan.actorId == battle->plan.targetIds[0])
          Message_ShowAndWaitCoreFar((s32)&MsgStopHittingYourself);
        else
          Message_ShowAndWaitCoreFar((s32)&MsgAttacksAlly);
      }
      Func_0811ea0c(&battle->plan, battle->plan.abilityId == 312 ? 32 : 0);
      swap = battle->plan.actorId;
      battle->plan.actorId = battle->plan.actorId2;
      battle->plan.actorId2 = swap;
      break;
    }
    case 11: {
      s32 kept;

      battle->skipPlayback = 1;
      Func_0811f088(&battle->plan, 1);
      battle->skipPlayback = 0;
      Message_ShowAndWaitCoreFar((s32)&MsgDjinnRecoverFaster);
      kept = Func_08124cc4(battle->plan.targetIds[0] > 7);
      if (kept == 0)
        Message_ShowAndWaitCoreFar((s32)(MsgDjinnRecoverFaster + 1));
      if (kept <= 1)
        BattleMessage_WaitForAdvance();
      break;
    }
    case 12:
      Func_0811df70(&battle->plan, 1);
      if (BattleParty_ListLivingUnits(1, 0) == 0)
        break;
      if (BattleParty_ListLivingUnits(2, 0) == 0)
        break;
      Func_0811d89c((struct ActionTargets *)&battle->plan);
      Func_0811f088(&battle->plan, 1);
      break;
    case 13:
      Func_0811df70(&battle->plan, 1);
      Func_0811d914((struct ActionTargets *)&battle->plan);
      Func_0811f088(&battle->plan, 1);
      break;
    case 7:
      if (Func_0811de78(&battle->plan) != 0)
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
  Func_0812561c(&battle->plan);
  BattleObject_Put();
  Enemy_Update();
  Func_0811d9a4();
  request->unitId = 255;
finish:
  Func_0812628c(
      2, ((struct DispatchBattleWork *)Ram_HeapSlots->battleWork)->background,
      0);
  Sound_PlayFar(390);
  return fled;
}

void Func_0811ddd8(struct BattlePlan *plan,
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
      if (BattleParty_IsUnitListed(*inputTarget) &&
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

struct EscapeTransition {
  s32 targetYaw;
  s32 unk04[3];
  s32 active;
};

/// Plays a flee action, The Broken Seal's BattleStaging_RunFlee without its
/// last wait: the party runs unless encounterMode is 2, and an enemy gets away
/// 7 times in 10. Returns 1 when the party got away.
s32 Func_0811de78(struct BattlePlan *plan) {
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
      Message_ShowAndWaitCoreFar((s32)MsgNoEscape);
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
    BattleActor_RemoveFromLists(plan->actorId);
    BattleObject_MarkForRelease(plan->actorId);
  } else {
    Message_ShowAndWaitCoreFar((s32)MsgNoEscape);
    BattleMessage_WaitForAdvance();
  }

  presentation->active = 0;
  return completed;
}

struct CastAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct CastAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct CastAnimationEntry *entries[1];
};

void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void BattleBackground_RestorePalette(void);
void BattlePlayback_WaitForReady(void);

s32 Func_0811df70(struct BattlePlan *plan, s32 mode) {
  u16 units[14];
  struct BattlePresentationWork work;
  u32 actor;
  u32 target;
  u32 count;
  u32 i;
  u32 kept;
  struct MotionObject *object;
  u16 *list;

  Func_0811ddd8(plan, &work);
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
      Actor_SetMode((struct Actor *)object,
                    ((u8 *)Ram_HeapSlots->battleWork)[0x869]);
    }
  }
  if (!(mode & 2)) {
    Sound_PlayFar(154);
    Func_08127308(work.actor, plan->element, 0, 0);
  }
  if (mode & 1)
    BattleHighlight_SetSpriteMode(actor, 1);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = (16 - i) | 0x1000;
    wait(1);
  }

  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattleEvent_Push(0, plan->actorId);
      BattleEvent_Push(4, (u32)&MsgPsynergyBlocked);
    } else {
      BattleEvent_Push(4, (u32)&MsgCastLacksPp);
    }
    BattleEvent_DispatchQueued();
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
    Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
    if (plan->presentationFlags & 0x8000)
      BattleEffect_RunSceneEffectFar((s32 *)&work);
    else if (plan->presentationFlags & 0x4000)
      Func_08138008(&work);
    else
      Func_08138018(&work);
    BattlePlayback_WaitForReady();
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
      BattleEvent_DispatchQueued();
    }
  }
  wait(1);
}
