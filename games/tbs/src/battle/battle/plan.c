// Part of battle.c, the battle object, compiled only through it.
// An action announced and planned, BattleCommand_BuildPlan.

#include "../battle_internal.h"

// Returns one when the unit is in its side's formation.
// unitId: 0 to 7 a party member, any other an enemy.
u32 BattleParty_IsUnitListed(u32 unitId) {
  u16 list[8];
  s32 side;
  s32 unitCount;
  s32 unitIndex;

  side = BATTLE_SIDE_PARTY;
  if (unitId > 7)
    side = BATTLE_SIDE_ENEMIES;
  unitCount = BattleParty_ListActorIds(side, list);

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    if (list[unitIndex] == unitId)
      break;
  }

  return unitIndex != unitCount;
}

// Counts standby Djinn owned by the selected unit's active battle side.
// Optionally writes one count per element.
s32 BattleParty_CountStandbyDjinn(u32 unitId, u8 *counts) {
  u16 unitIds[8];
  struct DjinnRecoveryList *recoveryList;
  s32 unitCount;
  s32 standbyCount;
  s32 recoveryIndex;
  s32 unitOrElementIndex;
  s32 sideMask;
  s32 enemySide;

  standbyCount = 0;
  sideMask = BATTLE_SIDE_PARTY;
  if (unitId > 7)
    sideMask = BATTLE_SIDE_ENEMIES;
  unitCount = BattleParty_ListActorIds(sideMask, unitIds);
  enemySide = 0;
  if (unitId > 7)
    enemySide = 1;
  recoveryList =
      &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(enemySide))
           ->list;
  if (counts != 0)
    for (unitOrElementIndex = 3; unitOrElementIndex >= 0; unitOrElementIndex--)
      counts[unitOrElementIndex] = 0;
  recoveryIndex = 0;
  if (recoveryList->count != 0) {
    do {
      if (recoveryList->entries[recoveryIndex].turns == -1) {
        for (unitOrElementIndex = 0; unitOrElementIndex < unitCount;
             unitOrElementIndex++)
          if (unitIds[unitOrElementIndex] ==
              recoveryList->entries[recoveryIndex].unitId)
            break;
        if (unitOrElementIndex != unitCount) {
          if (counts != 0)
            counts[recoveryList->entries[recoveryIndex].element]++;
          standbyCount++;
        }
      }
      recoveryIndex++;
    } while (recoveryIndex != recoveryList->count);
  }
  return standbyCount;
}

// Djinni selector: element in bits 8..11, index in bits 0..7.
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)
#define TARGET_MAX 14

// Announces the action and plans its targets and effects.
// Returns zero to execute, -1 on failure (already reported), or -2 for no
// action.
s32 BattleCommand_BuildPlan(struct BattleActionRecord *request,
                            struct BattlePlan *plan) {
  struct BattleUnit *actor = UnitGetFar(request->unitId);
  struct BattleWork *battle = gBattleWork;
  s32 selectedTargetId;
  s32 abilityId;

  //----------------------------------------------------------------------
  // List targets within the ability's range; defend and return -1 if none
  // exist.
  //----------------------------------------------------------------------
  s32 BattlePlan_SetTargets(s32 abilityId) {
    struct BattleAct *ability = BattleActGet(abilityId);
    s32 mode = ability->targetMode;
    s32 includeDowned = 0;
    s32 targetCount;
    s32 partyCount;
    s32 enemyCount;
    s32 aimedPlace;
    s32 range;
    s32 place;
    s32 targetId;

    switch (ability->effect) {
    case ABILITY_EFFECT_REVIVE:
    case ABILITY_EFFECT_REVIVE_HALF:
    case ABILITY_EFFECT_REVIVE_80:
      includeDowned = 1;
      break;
    }
    switch (mode) {
    case 0: // Single target.
      plan->targetOffset[0] = mode;
      plan->targetCount = 1;
      plan->targetIds[0] = selectedTargetId;
      plan->targetMultiplier[0] = 1;
      break;
    case 4:
      plan->targetOffset[0] = 0;
      plan->targetCount = 1;
      plan->targetIds[0] = selectedTargetId;
      plan->targetMultiplier[0] = 1;
      break;
    default:
      // Formation positions through range - 1 from the selected position.
      targetCount = 0;
      for (place = 0; battle->partyUnits[place] != BATTLE_UNIT_LIST_END;
           place++)
        ;
      partyCount = place;
      for (place = 0; battle->enemyIds[place] != BATTLE_UNIT_LIST_END; place++)
        ;
      enemyCount = place;
      aimedPlace = request->target & 15;
      range = request->range;
      {
        s32 first = aimedPlace - range + 1;
        s32 last = aimedPlace + range - 1;

        for (place = first; place <= last; place++) {
          if (place < 0)
            continue;
          // The enemies' side.
          if (request->target & 128) {
            if (place >= enemyCount)
              continue;
            targetId = battle->enemyIds[place];
            if (targetId == BATTLE_UNIT_REMOVED)
              continue;
            if (!includeDowned && UnitGetFar(targetId)->hp == 0)
              continue;
            plan->targetMultiplier[targetCount] = 1;
            plan->targetOffset[targetCount] = place - aimedPlace;
            plan->targetIds[targetCount] = targetId;
            targetCount++;
          } else {
            if (place >= partyCount)
              continue;
            targetId = battle->partyUnits[place];
            if (targetId == BATTLE_UNIT_REMOVED)
              continue;
            if (!includeDowned && UnitGetFar(targetId)->hp == 0)
              continue;
            plan->targetMultiplier[targetCount] = 1;
            plan->targetOffset[targetCount] = place - aimedPlace;
            plan->targetIds[targetCount] = targetId;
            targetCount++;
          }
        }
      }
      plan->targetCount = targetCount;
      if (targetCount <= 0) {
        UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
        UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
        if (actor->guardLevel == 0)
          actor->guardLevel = GUARD_DEFEND;
        return -1;
      }
    }
#ifdef UBFIX
    // Callers test only -1. A plain success return shortens EN by four bytes.
    return plan->targetCount;
#endif
  }

  selectedTargetId = Battle_GetTaggedSlotValue(request->target);
  BattleEventRuntime_Reset();
  plan->hauntDamage = 0;
  plan->actorId = request->unitId;
  plan->targetCount = 0;
  plan->presentationFlags = 0;
  plan->failure = 0;
  plan->element = 4;
  UiWork_ClearValueNameTablesFar();
  if (actor->hp == 0)
    return -2;

  // Debug: hold R to defeat all enemies, or R+Select to defeat the whole party.
  if (BATTLE_AUTO_MODE && GameFlag_TestFar(0x16d) && (BATTLE_OPTIONS & KEY_R)) {
    s32 enemy = 1;
    s32 index;
    s32 id;

    if (BATTLE_OPTIONS & KEY_SELECT)
      enemy = 0;
    for (index = 0;; index++) {
      if (enemy)
        id = battle->enemyIds[index];
      else
        id = battle->partyUnits[index];
      if (id == BATTLE_UNIT_LIST_END)
        break;
      if (id == BATTLE_UNIT_REMOVED)
        continue;
      if (Unit_AddHpFar(id, 0xc0000000) == 0) {
        BattleEvPush(BATTLE_EVENT_HIT, id);
        BattleEvPush(BATTLE_EVENT_DEFEAT, id);
      }
    }
    BattleEv_DispatchQueued();
    return -2;
  }

  // Conditions that prevent acting.
  UiWork_ClearValueNameTablesFar();
  if (actor->immobile) {
    actor->immobile = 0;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgUnableToMove);
    return -1;
  }
  if (actor->sleep) {
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsAsleep);
    return -1;
  }
  if (actor->stun) {
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsParalyzed);
    return -1;
  }
  // Paralysis prevents acting one quarter of the time.
  if ((actor->restraint & 1) && request->command != CMD_DEFEND &&
      (BattleRandomFar() & 3) == 0) {
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsBound);
    return -1;
  }
  if (request->command == CMD_NONE)
    return -2;
  abilityId = ABILITY_ATTACK;
  {
    s32 index;

    for (index = 0; index < TARGET_MAX; index++)
      plan->targetCritical[index] = 0;
    for (index = 0; index < TARGET_MAX; index++)
      plan->targetHit[index] = -1;
  }

  //----------------------------------------------------------------------
  // Announce the selected action.
  //----------------------------------------------------------------------
  switch (request->command) {
  case CMD_FLEE:
    if ((u16)request->unitId <= ALLY_MAX) {
      UiText_ShowMessageAndWaitCoreFar(MsgPartyFlees);
    } else {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgActorRuns);
    }
    BattlePresentation_WaitForAdvance();
    plan->outcome = PLAN_SHOW_FLEE;
    return 0;

  case CMD_ATTACK:
    // A weapon unleash replaces the action with its ability.
    abilityId = RollWeaponUnleashFar(actor);
    if (BattlePlan_SetTargets(abilityId) == -1)
      return -1;
    if (abilityId != ABILITY_ATTACK) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(Inventory_GetEquippedItemFar(actor, 1),
                              VALUE_ITEM_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgWeaponHowls);
      BattleEv_MarkDisplay();
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgWeaponHowls + 1);
    }
    break;

  case CMD_PSYNERGY: {
    struct BattleAct *ability;
    s32 canCast = 1;

    abilityId = request->parameter;
    ability = BattleActGet(abilityId);
    if (BattlePlan_SetTargets(abilityId) == -1)
      return -1;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorCasts);
    // Fail if PP is insufficient or Psynergy is sealed.
    if (actor->pp < ability->ppCost) {
      plan->failure = 2;
      canCast = 0;
    }
    if (actor->psySeal) {
      plan->failure = 1;
      canCast = 0;
    }
    if (canCast) {
      plan->failure = 0;
      actor->pp -= ability->ppCost;
      UnitRatioFar(request->unitId);
      CLAMP(actor->pp, 0, actor->maxPp);
    }
  } break;

  case CMD_ITEM: {
    struct ItemDefinition *item;

    if (request->parameter < 0) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgItemAlreadyUsed);
      return -1;
    }
    item = Item_GetDirectFar(actor->inventory[request->parameter]);
    abilityId = item->actionId;
    if (abilityId == 0 || (actor->inventory[request->parameter] & 0x400)) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
      if (actor->guardLevel == 0)
        actor->guardLevel = GUARD_DEFEND;
      return -1;
    }
    if (BattlePlan_SetTargets(abilityId) == -1)
      return -1;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(actor->inventory[request->parameter],
                            VALUE_ITEM_NAME);
    if (item->useType == ITEM_USE_MAY_BREAK || item->useType == 0) {
      switch (item->type) {
      case 1:
      case 3:
      case 6:
      case 7:
      case 8:
        UiText_ShowMessageAndWaitCoreFar(MsgActorRaisesItem);
        break;
      default:
        UiText_ShowMessageAndWaitCoreFar(MsgActorUsesBattleItem);
        break;
      }
    } else {
      UiText_ShowMessageAndWaitCoreFar(MsgActorUsesBattleItem);
    }
  } break;

  case CMD_DEFEND:
  case CMD_DEFEND2:
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
    return -1;

  case CMD_NONE:
    return -2;

  case CMD_ABILITY: {
    s32 message;

    abilityId = request->parameter;
    if (BattlePlan_SetTargets(abilityId) == -1)
      return -1;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    if ((BattleActGet(abilityId)->flags & ABILITY_KIND_MASK) ==
        ABILITY_KIND_UNLEASHED_DAMAGE)
      message = MsgActorUnleashesAbility;
    else
      message = MsgActorUsesAbility;
    // Select the wording for each ability.
    switch (abilityId) {
    case 435: // Ultrasonic waves.
    case 436:
      message = MsgEmitsUltrasonicWaves;
      break;
    case 224: // Heat mirage.
      message = MsgActorCasts;
      break;
    case 500: // Intimidation.
      message = MsgGlowersFerociously;
      break;
    case 501: // Trembling hands.
      message = MsgIsIntimidated;
      break;
    case 499: // Threatening gaze.
      message = MsgBalefulGaze;
      break;
    case 494: // Eating worms.
      message = MsgEatsWorms;
      break;
    case 437: // Roaring and howling abilities.
    case 438:
    case 439:
    case 440:
    case 441:
      message = MsgLetsOutAbility;
      break;
    case 442: // Wing attacks.
    case 443:
    case 444:
      message = MsgActorUsesAbility;
      break;
    case 472: // Rotten bodily fluid.
      message = MsgEnemyUnleashesAbility;
      break;
    case 488: // Sorrowful gaze.
      message = MsgGlowersMiserably;
      break;
    case 492: // Stench of decay.
      message = MsgSmellOfDecay;
      break;
    case 495: // Becoming enraged.
      message = MsgFuriousRage;
      break;
    case 503: // Attempting to divide.
      message = MsgAttemptsToDivide;
      break;
    case 504: // Looking for allies.
      message = MsgLooksForAllies;
      break;
    case 508: // Looking for help.
      message = MsgLooksForHelp;
      break;
    }
    UiText_ShowMessageAndWaitCoreFar(message);
  } break;

  case CMD_DJINN:
    // If not set, return the standby Djinni to its set state.
    abilityId = Djinn_GetAbilityIdFar(DJINN_ELEMENT(request->parameter),
                                      DJINN_INDEX(request->parameter));
    if (!Djinn_IsActiveFar(request->unitId, DJINN_ELEMENT(request->parameter),
                           DJINN_INDEX(request->parameter))) {
      if (Djinn_IsReadyFar(request->unitId, DJINN_ELEMENT(request->parameter),
                           DJINN_INDEX(request->parameter))) {
        BattleActGet(abilityId);
        BattlePres_SetActorModes(0, 0);
        Djinn_ActivateFar(request->unitId, DJINN_ELEMENT(request->parameter),
                          DJINN_INDEX(request->parameter));
        DjinnRecovery_RemoveFar(request->unitId,
                                DJINN_ELEMENT(request->parameter),
                                DJINN_INDEX(request->parameter));
        UnitCalcFar(request->unitId);
        BattleEventRuntime_Reset();
        BattleEventRuntime_SchedulePhase(30);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, request->unitId);
        BattleEvPush(BATTLE_EVENT_ABILITY_NAME,
                     DJINN_ELEMENT(request->parameter) * 20 +
                         DJINN_INDEX(request->parameter) + 300);
        BattleEvPush(BATTLE_EVENT_SOUND, SE_175);
        BattleEvPush(BATTLE_EVENT_DRAW_STATUS, 0);
        BattleEvPush(BATTLE_EVENT_MESSAGE, MsgDjinnSet);
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, request->unitId);
        AudioCommand_PlayFar(SE_212);
        ObjectDispatch_ApplyArgumentToChildrenFar(
            BattleObjGet(request->unitId)->object, 3);
        ObjectDispatch_ApplyValueToChildrenFar(
            (struct DispatchObject *)BattleObjGet(request->unitId)->object, 32);
        BattleFx_PlayUnitElementEffect(request->unitId,
                                       DJINN_ELEMENT(request->parameter), 3, 0);
        BattleEventRuntime_WaitForReady();
        return -2;
      }
      // Recovering.
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      AudioCommand_PlayFar(SE_MENU_ERROR);
      UiText_ShowMessageAndWaitCoreFar(MsgDjinnInRecovery);
      WaitFrames(60);
      return -1;
    }
    // Unleash the set Djinni.
    {
      struct BattleAct *ability;

      if (BattlePlan_SetTargets(abilityId) == -1)
        return -1;
      DjinnRecovery_AddStandbyFar(request->unitId,
                                  DJINN_ELEMENT(request->parameter),
                                  DJINN_INDEX(request->parameter));
      ability = BattleActGet(abilityId);
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgActorUnleashesDjinn);
      plan->element = ability->element;
    }
    break;

  case CMD_SUMMON: {
    const struct SummonDefinition *summon;
    struct DjinnRecoveryList *list;
    s32 side;
    u8 counts[4];
    /* FAKEMATCH(dead): an unused array for the frame's 8 bytes below counts;
     * without it, 34 of 1393 instructions differ in tbs-en. */
    u8 unused[8];
    s32 index;

    // Check whether enough Djinn of each element are on standby.
    summon = SummonDefinition_Get(request->parameter);
    BattleParty_CountStandbyDjinn(request->unitId, counts);
    side = 0;
    if ((u16)request->unitId > ALLY_MAX)
      side = 1;
    list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
    for (index = 0; index < 4 && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->nameMessageId;
    if (BattlePlan_SetTargets(abilityId) == -1)
      return -1;
    if (index != 4) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgSummonLacksDjinn);
      return -1;
    }
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorSummons);
    // Mark the standby Djinn being used with -2.
    for (index = 0; index != list->count; index++) {
      struct DjinnRecoveryEntry *entry = &list->entries[index];

      if (entry->turns == -1 && BattleParty_IsUnitListed(entry->unitId) &&
          counts[entry->element] != 0) {
        entry->turns = -2;
        counts[entry->element]--;
      }
    }
  } break;
  }

  //----------------------------------------------------------------------
  // Plan a normal attack.
  //----------------------------------------------------------------------
  if (abilityId == ABILITY_ATTACK) {
    struct BattleUnit *target = UnitGetFar(plan->targetIds[0]);

    plan->abilityId = ABILITY_ATTACK;
    plan->element = Item_GetEquippedElementFar(request->unitId);
    plan->outcome = PLAN_SHOW_ATTACK;
    // An enemy's attack effect comes from its class.
    if (actor->classIndex == 0) {
      plan->presentationFlags =
          BattleClass_GetAttackEffect(actor->classId) | STAGING_BY_ID_RANGE;
    } else {
      plan->presentationFlags = 0;
      switch (actor->classId) {
      case ISAAC:
        plan->presentationFlags = STAGING_BY_ID_RANGE | 1;
        break;
      case GARET:
        plan->presentationFlags = STAGING_BY_ID_RANGE | 1;
        break;
      case IVAN:
        plan->presentationFlags = STAGING_BY_ID_RANGE | 4;
        break;
      case MIA:
        plan->presentationFlags = STAGING_BY_ID_RANGE | 4;
        break;
      case JENNA:
        plan->presentationFlags = STAGING_BY_ID_RANGE | 1;
        break;
      }
    }
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorAttacks);
    // Miss chance: 153/256 when deluded, plus a separate 1/32 roll.
    // Attacks cannot miss a target that cannot move.
    /* FAKEMATCH(one-pass): the rolls in a one-pass block left by break, which
     * places them ahead of the five tests; as an if on the negated tests, 46
     * of 1393 instructions differ in tbs-en. */
    do {
      if (target->hp == 0 || target->sleep != 0 || target->stun != 0 ||
          target->immobile != 0 || target->charm != 0)
        break;
      if (actor->delusion != 0 && (BattleRandomFar() & 255) <= 152)
        plan->targetMultiplier[0] = 0;
      if ((BattleRandomFar() & 31) == 0)
        plan->targetMultiplier[0] = 0;
    } while (0);
    if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL))
      plan->targetMultiplier[0] = 0;
    // Critical hit: one chance in 32, or the equipment's chance.
    if (target->hp != 0) {
      if ((BattleRandomFar() & 31) == 0)
        plan->targetCritical[0] = 1;
      else if ((Equipment_GetUnleashRateBonusFar(actor) << 16) / 200 >
               (BattleRandomFar() & 0xffff))
        plan->targetCritical[0] = 1;
    }
  }

  //----------------------------------------------------------------------
  // Plan an ability.
  //----------------------------------------------------------------------
  else {
    struct BattleAct *ability = BattleActGet(abilityId);

    plan->element = ability->element;
    plan->presentationFlags = 0;
    plan->abilityId = abilityId;
    if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_60 ||
        ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_1_IN_8 ||
        ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4 ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_1_IN_8 ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_1_IN_4 ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_60) {
      // Chance to increase the damage multiplier.
      s32 chance;
      s32 amount;

      if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_60 ||
          ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_60)
        chance = 153;
      else if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_1_IN_8 ||
               ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_1_IN_8)
        chance = 32;
      else
        chance = 64;
      if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_60 ||
          ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_1_IN_8 ||
          ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4)
        amount = 1;
      else
        amount = 2;
      if ((BattleRandomFar() & 255) < chance) {
        s32 index;

        for (index = 0; index < plan->targetCount; index++)
          plan->targetMultiplier[index] += amount;
      }
    } else if (ability->effect >= ABILITY_EFFECT_CRITICAL_1_IN_64 &&
               ability->effect <= ABILITY_EFFECT_CRITICAL_1_IN_4) {
      // Critical-hit chance ranges from 1/64 to 1/4.
      s32 mask;

      switch (ability->effect) {
      case 36:
        mask = 63;
        break;
      case 37:
        mask = 31;
        break;
      case 38:
        mask = 15;
        break;
      case 39:
        mask = 7;
        break;
      case 40:
      default:
        mask = 3;
        break;
      }
      if ((BattleRandomFar() & mask) == 0) {
        s32 index;

        for (index = 0; index < plan->targetCount; index++)
          plan->targetCritical[index] = 2;
      }
    } else if (abilityId == 178) {
      // Ability 178 rolls its hit now, as the plan is made.
      s32 index;

      for (index = 0; index < plan->targetCount; index++)
        plan->targetHit[index] =
            BattleHitCheck(request->unitId, plan->targetIds[index],
                           ability->element, ability->effect, 100);
    }
    if (abilityId >= 0 && abilityId <= 518) {
      plan->presentationFlags = gBattle_ActionFlags[abilityId];
      if (plan->targetMultiplier[0] > 1)
        plan->presentationFlags = gBattle_ActionFlags[abilityId] +
                                  (plan->targetMultiplier[0] << 12) - 0x1000;
    }
    if ((abilityId >= 0 && abilityId <= 517) &&
        gBattle_ActionStatus[abilityId] != 0)
      plan->outcome = gBattle_ActionStatus[abilityId];
    else if (Ability_IsPsynergy(abilityId))
      plan->outcome = PLAN_SHOW_CAST;
    else if (plan->presentationFlags != 0) {
      if (actor->classIndex == 0)
        plan->outcome = PLAN_SHOW_ENEMY_CAST;
      else
        plan->outcome = PLAN_SHOW_CAST;
    } else
      plan->outcome = PLAN_SHOW_MESSAGES;
    if (Battle_IsReviveEffectFar(ability->effect))
      plan->presentationFlags |= STAGING_ON_DOWNED;
    if (abilityId == 178 && plan->targetHit[0] != 0)
      plan->presentationFlags |= 1 << STAGING_VARIANT_SHIFT;
  }
  if (request->command == CMD_ITEM && plan->outcome != PLAN_SHOW_SUMMON &&
      plan->outcome != PLAN_SHOW_FULL_SCREEN)
    plan->outcome = PLAN_SHOW_ITEM;
  plan->command = request->command;
  return 0;
}
