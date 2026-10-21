/// @file
/// An action announced and planned, BattleCommand_BuildPlan.
// Part of battle.c.

#include "../battle_internal.h"

/// Returns 1 when the unit is in its side's list of units in the battle
/// (BattleParty_ListActorIds), 0 otherwise.
/// Called by BattleCommand_BuildPlan, for the owners of the standby Djinn a
/// summon uses.
/// @param unitId a party member's, up to ALLY_MAX, or an enemy's
u32 BattleParty_IsUnitListed(u32 unitId) {
  u16 list[8];
  s32 side;
  s32 unitCount;
  s32 unitIndex;

  side = BATTLE_SIDE_PARTY;
  if (unitId > ALLY_MAX)
    side = BATTLE_SIDE_ENEMIES;
  unitCount = BattleParty_ListActorIds(side, list);

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    if (list[unitIndex] == unitId)
      break;
  }

  return unitIndex != unitCount;
}

/// Counts the standby Djinn whose owners fight on unitId's side, from that
/// side's Djinn recovery list.
/// Called by BattleCommand_BuildPlan for a summon, and through
/// BattleParty_CountStandbyDjinnFar by the command menu (Menu_RunCommandRow,
/// Battle_CollectPartyCommands, PartyStatusDraw). Returns the total.
/// @param unitId a unit of the party's side, up to ALLY_MAX, or of the
/// enemies'
/// @param counts if not null, receives the count for each of the four
/// elements
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
  if (unitId > ALLY_MAX)
    sideMask = BATTLE_SIDE_ENEMIES;
  unitCount = BattleParty_ListActorIds(sideMask, unitIds);
  enemySide = DJINN_RECOVERY_PARTY;
  if (unitId > ALLY_MAX)
    enemySide = DJINN_RECOVERY_OPPONENT;
  recoveryList =
      &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(enemySide))
           ->list;
  if (counts != 0)
    for (unitOrElementIndex = NUM_ELEMENTS - 1; unitOrElementIndex >= 0;
         unitOrElementIndex--)
      counts[unitOrElementIndex] = 0;
  recoveryIndex = 0;
  if (recoveryList->count != 0) {
    do {
      if (recoveryList->entries[recoveryIndex].turns ==
          DJINN_RECOVERY_AFTER_BATTLE) {
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

// CMD_DJINN's parameter: the Djinni's element in bits 11..8, its index in
// bits 7..0.
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)
/// The most targets a plan holds (BATTLE_TARGET_CAPACITY).
#define TARGET_MAX 14
/// Announces the action and plans its targets and effects.
/// Returns 0 to stage the plan, PLAN_REPORTED when the action failed (its
/// message shown), or PLAN_NO_ACTION.
/// Called by BattlePresentation_DispatchAction for each action of the round.
/// Fills plan (its actor, targets, hit and critical rolls, ability, element,
/// outcome, presentation flags and failure), resets the battle events and
/// shows the announcing message. The command's costs are paid here: a
/// caster's PP, a Djinni set or put on standby (with its own effect), the
/// standby Djinn a summon uses (DJINN_RECOVERY_SUMMONED). A unit that cannot
/// act, or finds no target, may be set to guard (guardLevel).
///
/// The phases: the debug defeat; what stops the actor; the announcement and
/// the ability id, by command; the rolls of a plain attack, or an ability's
/// extra blows, critical hit or advance hit roll; then how it is staged.
s32 BattleCommand_BuildPlan(struct BattleActionRecord *request,
                            struct BattlePlan *plan) {
  struct BattleUnit *actor = UnitGetFar(request->unitId);
  struct BattleWork *battle = gBattleWork;
  s32 selectedTargetId;
  s32 abilityId;

  /// Fills the plan's targets: the aimed unit, or for a range the places
  /// within range - 1 of the aimed place on its side, standing units only
  /// unless the ability revives. With none, the actor defends ("defends")
  /// and it returns PLAN_REPORTED.
  /// Called by BattleCommand_BuildPlan once the command gives its ability id.
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
    // No target mode, and the user alone: the aimed unit, as one target.
    switch (mode) {
    case TARGET_MODE_NONE:
      plan->targetOffset[0] = mode;
      plan->targetCount = 1;
      plan->targetIds[0] = selectedTargetId;
      plan->targetMultiplier[0] = 1;
      break;
    case TARGET_MODE_SELF:
      plan->targetOffset[0] = 0;
      plan->targetCount = 1;
      plan->targetIds[0] = selectedTargetId;
      plan->targetMultiplier[0] = 1;
      break;
    default:
      // The places from range - 1 before the aimed place to range - 1 after
      // it, on the side the target's place code names (bit 7 the enemies',
      // the low four bits the place), within that side's list.
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
        return PLAN_REPORTED;
      }
    }
#ifdef UBFIX
    // Callers test only -1. A plain success return shortens EN by four bytes.
    return plan->targetCount;
#endif
  }

  //----------------------------------------------------------------------
  // A fresh plan, and the debug defeat
  //----------------------------------------------------------------------
  selectedTargetId = Battle_GetTaggedSlotValue(request->target);
  BattleEventRuntime_Reset();
  plan->hauntDamage = 0;
  plan->actorId = request->unitId;
  plan->targetCount = 0;
  plan->presentationFlags = 0;
  plan->failure = PLAN_FAILURE_NONE;
  plan->element = ELEMENT_NONE;
  UiWork_ClearValueNameTablesFar();
  if (actor->hp == 0)
    return PLAN_NO_ACTION;

  // Debug: hold R to defeat all enemies, or R+Select to defeat the whole party.
  // Only in debug mode, with flag 0x16d set; each unit loses 0x40000000 HP,
  // more than any unit has. Flag 0x16d is set by Vale's debug entrance
  // (ValeGate_SetProgressFlagsAndExit) and by the Colosseum's escorts as Mia
  // takes Jenna's place (colosseum_entrance.c). XXX: what the flag means; a
  // reader that tests it outside this debug key would say.
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
    return PLAN_NO_ACTION;
  }

  //----------------------------------------------------------------------
  // What stops the actor: Ground, sleep, stun, a bound unit's bad luck
  //----------------------------------------------------------------------
  UiWork_ClearValueNameTablesFar();
  if (actor->immobile) {
    actor->immobile = 0;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgUnableToMove);
    return PLAN_REPORTED;
  }
  if (actor->sleep) {
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsAsleep);
    return PLAN_REPORTED;
  }
  if (actor->stun) {
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsParalyzed);
    return PLAN_REPORTED;
  }
  // A bound unit (restraint bit 0) loses one action in four,
  // except Defend.
  if ((actor->restraint & 1) && request->command != CMD_DEFEND &&
      (BattleRandomFar() & 3) == 0) {
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgIsBound);
    return PLAN_REPORTED;
  }
  if (request->command == CMD_NONE)
    return PLAN_NO_ACTION;
  abilityId = ABILITY_ATTACK;
  {
    s32 index;

    for (index = 0; index < TARGET_MAX; index++)
      plan->targetCritical[index] = 0;
    for (index = 0; index < TARGET_MAX; index++)
      plan->targetHit[index] = HIT_NOT_ROLLED;
  }

  //----------------------------------------------------------------------
  // Announce the selected action and find its ability id and targets. A
  // command that ends here returns PLAN_REPORTED when a message has shown why
  // (BattlePresentation_DispatchAction then waits for the player), or
  // PLAN_NO_ACTION.
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
    if (BattlePlan_SetTargets(abilityId) == PLAN_REPORTED)
      return PLAN_REPORTED;
    if (abilityId != ABILITY_ATTACK) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(Inventory_GetEquippedItemFar(actor, EQUIP_WEAPON),
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
    if (BattlePlan_SetTargets(abilityId) == PLAN_REPORTED)
      return PLAN_REPORTED;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorCasts);
    // Fail if PP is insufficient or Psynergy is sealed (the seal wins); the
    // failure is shown by the staging (plan->failure). Only a cast that goes
    // ahead pays its PP.
    if (actor->pp < ability->ppCost) {
      plan->failure = PLAN_FAILURE_NO_PP;
      canCast = 0;
    }
    if (actor->psySeal) {
      plan->failure = PLAN_FAILURE_SEALED;
      canCast = 0;
    }
    if (canCast) {
      plan->failure = PLAN_FAILURE_NONE;
      actor->pp -= ability->ppCost;
      UnitRatioFar(request->unitId);
      CLAMP(actor->pp, 0, actor->maxPp);
    }
  } break;

  case CMD_ITEM: {
    struct ItemDefinition *item;

    // A slot of -1: an earlier command this round used the item up
    // (BattlePres_RunItem).
    if (request->parameter < 0) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgItemAlreadyUsed);
      return PLAN_REPORTED;
    }
    // An item with no ability, or a broken one, and the user defends.
    item = Item_GetDirectFar(actor->inventory[request->parameter]);
    abilityId = item->actionId;
    if (abilityId == 0 ||
        (actor->inventory[request->parameter] & INVENTORY_BROKEN)) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
      if (actor->guardLevel == 0)
        actor->guardLevel = GUARD_DEFEND;
      return PLAN_REPORTED;
    }
    if (BattlePlan_SetTargets(abilityId) == PLAN_REPORTED)
      return PLAN_REPORTED;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(actor->inventory[request->parameter],
                            VALUE_ITEM_NAME);
    // An item that is kept: weapons, shields, gloves and armlets, the Psynergy
    // items, rings and type 7, which no row of items.tsv has, are raised
    // ("raises"); anything else is used.
    if (item->useType == ITEM_USE_MAY_BREAK || item->useType == 0) {
      switch (item->type) {
      case ITEM_TYPE_WEAPON:
      case ITEM_TYPE_SHIELD:
      case ITEM_TYPE_PSYNERGY:
      case 7:
      case ITEM_TYPE_RING:
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
    return PLAN_REPORTED;

  case CMD_NONE:
    return PLAN_NO_ACTION;

  case CMD_ABILITY: {
    s32 message;

    abilityId = request->parameter;
    if (BattlePlan_SetTargets(abilityId) == PLAN_REPORTED)
      return PLAN_REPORTED;
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    if ((BattleActGet(abilityId)->flags & ABILITY_KIND_MASK) ==
        ABILITY_KIND_UNLEASHED_DAMAGE)
      message = MsgActorUnleashesAbility;
    else
      message = MsgActorUsesAbility;
    // Some enemy abilities have their own wording.
    switch (abilityId) {
    case ABILITY_SONIC_WAVE:
    case ABILITY_SONIC_WAVE_2:
      message = MsgEmitsUltrasonicWaves;
      break;
    case ABILITY_HEAT_MIRAGE:
      message = MsgActorCasts;
      break;
    case ABILITY_THREATEN:
      message = MsgGlowersFerociously;
      break;
    case ABILITY_TREMBLE:
      message = MsgIsIntimidated;
      break;
    case ABILITY_CONTAIN:
      message = MsgBalefulGaze;
      break;
    case ABILITY_WORMS:
      message = MsgEatsWorms;
      break;
    case ABILITY_SHRIEK:
    case ABILITY_BANSHEE_HOWL:
    case ABILITY_CRAZY_VOICE:
    case ABILITY_WAR_CRY:
    case ABILITY_WICKED_HOWL:
      message = MsgLetsOutAbility;
      break;
    case ABILITY_WING_BEAT:
    case ABILITY_WING_FLUTTER:
    case ABILITY_WING_STROKE:
      message = MsgActorUsesAbility;
      break;
    case ABILITY_ROTTEN_BLOOD:
      message = MsgEnemyUnleashesAbility;
      break;
    case ABILITY_HEARTRENDER:
      message = MsgGlowersMiserably;
      break;
    case ABILITY_DECOMPOSE:
      message = MsgSmellOfDecay;
      break;
    case ABILITY_BERSERK:
      message = MsgFuriousRage;
      break;
    case ABILITY_SPEED_SURGE:
      message = MsgAttemptsToDivide;
      break;
    case ABILITY_ALLY_SEARCH:
      message = MsgLooksForAllies;
      break;
    case ABILITY_SEARCH_FOR_HELP:
      message = MsgLooksForHelp;
      break;
    }
    UiText_ShowMessageAndWaitCoreFar(message);
  } break;

  case CMD_DJINN:
    // A Djinni not set: one ready again is set, with its own short scene
    // played through the event queue, and nothing else happens
    // (PLAN_NO_ACTION); one still recovering says so (PLAN_REPORTED). A set
    // Djinni is unleashed below.
    abilityId = Djinn_GetAbilityIdFar(DJINN_ELEMENT(request->parameter),
                                      DJINN_INDEX(request->parameter));
    if (!Djinn_IsActiveFar(request->unitId, DJINN_ELEMENT(request->parameter),
                           DJINN_INDEX(request->parameter))) {
      if (Djinn_IsReadyFar(request->unitId, DJINN_ELEMENT(request->parameter),
                           DJINN_INDEX(request->parameter))) {
        BattleActGet(abilityId);
        BattlePres_SetActorModes(0, HIGHLIGHT_OFF);
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
                     DJINN_ELEMENT(request->parameter) * DJINN_PER_ELEMENT +
                         DJINN_INDEX(request->parameter) +
                         ABILITY_DJINNI_FIRST);
        BattleEvPush(BATTLE_EVENT_SOUND, SE_175);
        BattleEvPush(BATTLE_EVENT_DRAW_STATUS, 0);
        BattleEvPush(BATTLE_EVENT_MESSAGE, MsgDjinnSet);
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, request->unitId);
        AudioCommand_PlayFar(SE_212);
        ObjectDispatch_SelectAnimationFar(BattleObjGet(request->unitId)->object,
                                          POSE_CAST);
        ObjectDispatch_SetAnimationSpeedFar(
            (struct DispatchObject *)BattleObjGet(request->unitId)->object, 32);
        BattleFx_PlayUnitElementEffect(
            request->unitId, DJINN_ELEMENT(request->parameter), GLOW_BURST, 0);
        BattleEventRuntime_WaitForReady();
        return PLAN_NO_ACTION;
      }
      // Recovering.
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      AudioCommand_PlayFar(SE_MENU_ERROR);
      UiText_ShowMessageAndWaitCoreFar(MsgDjinnInRecovery);
      WaitFrames(60);
      return PLAN_REPORTED;
    }
    // Unleash the set Djinni; it goes on standby.
    {
      struct BattleAct *ability;

      if (BattlePlan_SetTargets(abilityId) == PLAN_REPORTED)
        return PLAN_REPORTED;
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
    u8 counts[NUM_ELEMENTS];
    /* FAKEMATCH(dead): an unused array for the frame's 8 bytes below counts;
     * without it, 34 of 1393 instructions differ in tbs-en. */
    UNUSED u8 unused[8];
    s32 index;

    // Check whether enough Djinn of each element are on standby; the loop
    // stops at the first element short (index below 4), and leaves counts
    // holding what each element gives.
    summon = SummonDefinition_Get(request->parameter);
    BattleParty_CountStandbyDjinn(request->unitId, counts);
    side = DJINN_RECOVERY_PARTY;
    if ((u16)request->unitId > ALLY_MAX)
      side = DJINN_RECOVERY_OPPONENT;
    list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
    for (index = 0;
         index < NUM_ELEMENTS && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->nameMessageId;
    if (BattlePlan_SetTargets(abilityId) == PLAN_REPORTED)
      return PLAN_REPORTED;
    if (index != NUM_ELEMENTS) {
      UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
      UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
      UiText_ShowMessageAndWaitCoreFar(MsgSummonLacksDjinn);
      return PLAN_REPORTED;
    }
    UiWork_PushValueSlotFar(request->unitId, VALUE_UNIT_NAME);
    UiWork_PushValueSlotFar(abilityId, VALUE_ABILITY_NAME);
    UiText_ShowMessageAndWaitCoreFar(MsgActorSummons);
    // Mark the standby Djinn being used DJINN_RECOVERY_SUMMONED, the first
    // found of each element, from owners in the formation.
    for (index = 0; index != list->count; index++) {
      struct DjinnRecoveryEntry *entry = &list->entries[index];

      if (entry->turns == DJINN_RECOVERY_AFTER_BATTLE &&
          BattleParty_IsUnitListed(entry->unitId) &&
          counts[entry->element] != 0) {
        entry->turns = DJINN_RECOVERY_SUMMONED;
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
    // An enemy's attack effect comes from its class; a party member's is
    // effect 1 for Isaac, Garet and Jenna and 4 for Ivan and Mia, any other
    // none. Each is played by id range (STAGING_BY_ID_RANGE).
    if (actor->classIndex == CLASS_INDEX_ENEMY) {
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
    // Attacks cannot miss a target that cannot move. In the Djinn tutorial
    // the attack always misses.
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
    // Critical hit: one chance in 32, or else the equipment's unleash bonus
    // in 200ths, against a 16-bit roll.
    if (target->hp != 0) {
      if ((BattleRandomFar() & 31) == 0)
        plan->targetCritical[0] = CRITICAL_HIT;
      else if ((Equipment_GetUnleashRateBonusFar(actor) << 16) / 200 >
               (BattleRandomFar() & 0xffff))
        plan->targetCritical[0] = CRITICAL_HIT;
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
      // A double or triple blow on every target: 153, 32 or 64 in 256, the
      // multiplier rising by 1 or 2.
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
      // An ability's critical hit on every target, 1 in 64 to 1 in 4.
      s32 mask;

      switch (ability->effect) {
      case ABILITY_EFFECT_CRITICAL_1_IN_64:
        mask = 63;
        break;
      case ABILITY_EFFECT_CRITICAL_1_IN_32:
        mask = 31;
        break;
      case ABILITY_EFFECT_CRITICAL_1_IN_16:
        mask = 15;
        break;
      case ABILITY_EFFECT_CRITICAL_1_IN_8:
        mask = 7;
        break;
      case ABILITY_EFFECT_CRITICAL_1_IN_4:
      default:
        mask = 3;
        break;
      }
      if ((BattleRandomFar() & mask) == 0) {
        s32 index;

        for (index = 0; index < plan->targetCount; index++)
          plan->targetCritical[index] = CRITICAL_STRONG;
      }
    } else if (abilityId == ABILITY_ANNIHILATION) {
      // Annihilation rolls its hit now, at full rate whatever the distance,
      // so that the effect can show it (the variant below).
      s32 index;

      for (index = 0; index < plan->targetCount; index++)
        plan->targetHit[index] =
            BattleHitCheck(request->unitId, plan->targetIds[index],
                           ability->element, ability->effect, 100);
    }
    // How it is staged: the ability's row of actflags.tsv, its variant
    // raised to the first target's multiplier less one (a double or triple
    // blow); then the outcome, from actstat.tsv where it gives one. The two
    // tables end at ABILITY_THRASH and ABILITY_SEVERE_BLOW.
    if (abilityId >= 0 && abilityId <= ABILITY_THRASH) {
      plan->presentationFlags = gBattle_ActionFlags[abilityId];
      if (plan->targetMultiplier[0] > 1)
        plan->presentationFlags =
            gBattle_ActionFlags[abilityId] +
            (plan->targetMultiplier[0] << STAGING_VARIANT_SHIFT) -
            (1 << STAGING_VARIANT_SHIFT);
    }
    if ((abilityId >= 0 && abilityId <= ABILITY_SEVERE_BLOW) &&
        gBattle_ActionStatus[abilityId] != 0)
      plan->outcome = gBattle_ActionStatus[abilityId];
    else if (Ability_IsPsynergy(abilityId))
      plan->outcome = PLAN_SHOW_CAST;
    else if (plan->presentationFlags != 0) {
      if (actor->classIndex == CLASS_INDEX_ENEMY)
        plan->outcome = PLAN_SHOW_ENEMY_CAST;
      else
        plan->outcome = PLAN_SHOW_CAST;
    } else
      plan->outcome = PLAN_SHOW_MESSAGES;
    if (Battle_IsReviveEffectFar(ability->effect))
      plan->presentationFlags |= STAGING_ON_DOWNED;
    // When Annihilation's instant death lands on its first target, the
    // variant becomes 1: the burst then shows its second picture and plays a
    // sound (BattleFx_RunAnnihilation).
    if (abilityId == ABILITY_ANNIHILATION && plan->targetHit[0] != 0)
      plan->presentationFlags |= 1 << STAGING_VARIANT_SHIFT;
  }
  // An item is staged as an item, unless its ability takes the whole screen.
  if (request->command == CMD_ITEM && plan->outcome != PLAN_SHOW_SUMMON &&
      plan->outcome != PLAN_SHOW_FULL_SCREEN)
    plan->outcome = PLAN_SHOW_ITEM;
  plan->command = request->command;
  return 0;
}
