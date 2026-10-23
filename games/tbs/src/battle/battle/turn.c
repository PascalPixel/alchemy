/// @file
/// An action announced and built into a turn, BattleTurn_Build.
// Part of battle.c.

#include "../battle_internal.h"

/// Returns 1 when the unit is in its side's list of units in the battle
/// (BattleUnit_ListIds), 0 otherwise.
/// Called by BattleTurn_Build, for the units holding the standby Djinn a
/// summon uses.
/// @param unitId a party member's, up to ALLY_MAX, or an enemy's
u32 BattleUnit_IsListed(u32 unitId) {
  u16 list[8];
  s32 side;
  s32 unitCount;
  s32 unitIndex;

  side = BATTLE_SIDES_PARTY;
  if (unitId > ALLY_MAX)
    side = BATTLE_SIDES_ENEMIES;
  unitCount = BattleUnit_ListIds(side, list);

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    if (list[unitIndex] == unitId)
      break;
  }

  return unitIndex != unitCount;
}

/// Counts the standby Djinn held by units on unitId's side, from that
/// side's Djinn recovery list.
/// Called by BattleTurn_Build for a summon, and through
/// BattleDjinn_CountStandbyFar by the command menu (BattleCommandRow_Run,
/// Battle_CollectPartyCommands, PartyStatus_Draw). Returns the total.
/// @param unitId a unit of the party's side, up to ALLY_MAX, or of the
/// enemies'
/// @param counts if not null, receives the count for each of the four
/// elements
s32 BattleDjinn_CountStandby(u32 unitId, u8 *counts) {
  u16 unitIds[8];
  struct DjinnRecoveryList *list;
  s32 unitCount;
  s32 standbyCount;
  s32 recoveryIndex;
  s32 unitOrElementIndex;
  s32 sideMask;
  s32 enemySide;

  standbyCount = 0;
  sideMask = BATTLE_SIDES_PARTY;
  if (unitId > ALLY_MAX)
    sideMask = BATTLE_SIDES_ENEMIES;
  unitCount = BattleUnit_ListIds(sideMask, unitIds);
  enemySide = DJINN_RECOVERY_PARTY;
  if (unitId > ALLY_MAX)
    enemySide = DJINN_RECOVERY_OPPONENT;
  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(enemySide))
              ->list;
  if (counts != 0)
    for (unitOrElementIndex = ELEMENT_COUNT - 1; unitOrElementIndex >= 0;
         unitOrElementIndex--)
      counts[unitOrElementIndex] = 0;
  recoveryIndex = 0;
  if (list->count != 0) {
    do {
      if (list->entries[recoveryIndex].turns == DJINN_RECOVERY_AFTER_BATTLE) {
        for (unitOrElementIndex = 0; unitOrElementIndex < unitCount;
             unitOrElementIndex++)
          if (unitIds[unitOrElementIndex] ==
              list->entries[recoveryIndex].unitId)
            break;
        if (unitOrElementIndex != unitCount) {
          if (counts != 0)
            counts[list->entries[recoveryIndex].element]++;
          standbyCount++;
        }
      }
      recoveryIndex++;
    } while (recoveryIndex != list->count);
  }
  return standbyCount;
}

// COMMAND_DJINN's parameter: the Djinni's element in bits 11..8, its index in
// bits 7..0.
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)
/// The most targets a turn holds (BATTLE_TARGET_CAPACITY).
#define TARGET_MAX 14
/// Announces the action and builds its turn: its targets and animation.
/// Returns 0 to stage the turn, TURN_REPORTED when the action failed (its
/// message shown), or TURN_NO_ACTION.
/// Called by BattleStaging_DispatchAction for each action of the round.
/// Fills the turn (its actor, targets, hit and critical rolls, ability,
/// element, staging, animation word and failure), resets the playback and shows
/// the announcing message. The command's costs are paid here: a caster's PP, a
/// Djinni set or put on standby (with its own animation), the standby Djinn a
/// summon uses (DJINN_RECOVERY_SUMMONED). A unit that cannot act, or finds no
/// target, may be set to guard (guardLevel).
///
/// The phases: the debug defeat; what stops the actor; the announcement and
/// the ability id, by command; the rolls of a plain attack, or an ability's
/// extra blows, critical hit or advance hit roll; then how it is staged.
s32 BattleTurn_Build(struct BattleAction *request, struct BattleTurn *plan) {
  struct Unit *actor = Unit_GetFar(request->unitId);
  struct BattleWork *battle = gBattleWork;
  s32 selectedTargetId;
  s32 abilityId;

  /// Fills the turn's targets: the aimed unit, or for a range the places
  /// within range - 1 of the aimed place on its side, standing units only
  /// unless the ability revives. With none, the actor defends ("defends")
  /// and it returns TURN_REPORTED.
  /// Called by BattleTurn_Build once the command gives its ability id.
  s32 BattleTurn_SetTargets(s32 abilityId) {
    struct AbilityDefinition *ability = Ability_Get(abilityId);
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
    case ABILITY_TARGET_ONE:
      plan->targetOffset[0] = mode;
      plan->targetCount = 1;
      plan->targetIds[0] = selectedTargetId;
      plan->targetMultiplier[0] = 1;
      break;
    case ABILITY_TARGET_SELF:
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
            if (!includeDowned && Unit_GetFar(targetId)->hp == 0)
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
            if (!includeDowned && Unit_GetFar(targetId)->hp == 0)
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
        MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
        BattleMessage_ShowAndWaitFar(MsgActorDefends);
        if (actor->guardLevel == 0)
          actor->guardLevel = GUARD_DEFEND;
        return TURN_REPORTED;
      }
    }
#ifdef UBFIX
    // Callers test only -1. A plain success return shortens EN by four bytes.
    return plan->targetCount;
#endif
  }

  //----------------------------------------------------------------------
  // A fresh turn, and the debug defeat
  //----------------------------------------------------------------------
  selectedTargetId = Battle_GetTaggedSlotValue(request->target);
  BattlePlayback_Reset();
  plan->hauntDamage = 0;
  plan->actorId = request->unitId;
  plan->targetCount = 0;
  plan->animation = 0;
  plan->failure = TURN_FAILURE_NONE;
  plan->element = ELEMENT_NONE;
  MessageArgument_ClearAllFar();
  if (actor->hp == 0)
    return TURN_NO_ACTION;

  // Debug: hold R to defeat all enemies, or R+Select to defeat the whole party.
  // Only in debug mode, with FLAG_DEBUG_BATTLE_KEYS set; each unit loses
  // 0x40000000 HP, more than any unit has.
  if (BATTLE_AUTO_MODE && GameFlag_TestFar(FLAG_DEBUG_BATTLE_KEYS) &&
      (BATTLE_OPTIONS & KEY_R)) {
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
        BattlePlayback_Push(PLAYBACK_HIT, id);
        BattlePlayback_Push(PLAYBACK_DEFEAT, id);
      }
    }
    BattlePlayback_RunAll();
    return TURN_NO_ACTION;
  }

  //----------------------------------------------------------------------
  // What stops the actor: Ground, sleep, stun, a bound unit's bad luck
  //----------------------------------------------------------------------
  MessageArgument_ClearAllFar();
  if (actor->immobile) {
    actor->immobile = 0;
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgUnableToMove);
    return TURN_REPORTED;
  }
  if (actor->sleep) {
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgIsAsleep);
    return TURN_REPORTED;
  }
  if (actor->stun) {
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgIsParalyzed);
    return TURN_REPORTED;
  }
  // A bound unit (curse bit 0) loses one action in four,
  // except Defend.
  if ((actor->curse & CURSE_BOUND) && request->command != COMMAND_DEFEND &&
      (Battle_RandomFar() & 3) == 0) {
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgIsBound);
    return TURN_REPORTED;
  }
  if (request->command == COMMAND_NONE)
    return TURN_NO_ACTION;
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
  // command that ends here returns TURN_REPORTED when a message has shown why
  // (BattleStaging_DispatchAction then waits for the player), or
  // TURN_NO_ACTION.
  //----------------------------------------------------------------------
  switch (request->command) {
  case COMMAND_FLEE:
    if ((u16)request->unitId <= ALLY_MAX) {
      BattleMessage_ShowAndWaitFar(MsgPartyFlees);
    } else {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar(MsgActorRuns);
    }
    BattleMessage_WaitForAdvance();
    plan->staging = STAGING_FLEE;
    return 0;

  case COMMAND_ATTACK:
    // A weapon unleash replaces the action with its ability.
    abilityId = RollWeaponUnleashFar(actor);
    if (BattleTurn_SetTargets(abilityId) == TURN_REPORTED)
      return TURN_REPORTED;
    if (abilityId != ABILITY_ATTACK) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(Inventory_GetEquippedItemFar(actor, EQUIP_WEAPON),
                              MESSAGE_ARGUMENT_ITEM);
      BattleMessage_ShowAndWaitFar(MsgWeaponHowls);
      BattlePlayback_MarkMessage();
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      BattleMessage_ShowAndWaitFar(MsgWeaponHowls + 1);
    }
    break;

  case COMMAND_PSYNERGY: {
    struct AbilityDefinition *ability;
    s32 canCast = 1;

    abilityId = request->parameter;
    ability = Ability_Get(abilityId);
    if (BattleTurn_SetTargets(abilityId) == TURN_REPORTED)
      return TURN_REPORTED;
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
    BattleMessage_ShowAndWaitFar(MsgActorCasts);
    // Fail if PP is insufficient or Psynergy is sealed (the seal wins); the
    // failure is shown by the staging (plan->failure). Only a cast that goes
    // ahead pays its PP.
    if (actor->pp < ability->ppCost) {
      plan->failure = TURN_FAILURE_NO_PP;
      canCast = 0;
    }
    if (actor->psySeal) {
      plan->failure = TURN_FAILURE_SEALED;
      canCast = 0;
    }
    if (canCast) {
      plan->failure = TURN_FAILURE_NONE;
      actor->pp -= ability->ppCost;
      Unit_RecalculateGaugesFar(request->unitId);
      CLAMP(actor->pp, 0, actor->ppMax);
    }
  } break;

  case COMMAND_ITEM: {
    struct ItemDefinition *item;

    // A slot of -1: an earlier command this round used the item up
    // (BattleStaging_RunItem).
    if (request->parameter < 0) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar(MsgItemAlreadyUsed);
      return TURN_REPORTED;
    }
    // An item with no ability, or a broken one, and the user defends.
    item = Item_GetDirectFar(actor->inventory[request->parameter]);
    abilityId = item->abilityId;
    if (abilityId == 0 ||
        (actor->inventory[request->parameter] & INVENTORY_BROKEN)) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar(MsgActorDefends);
      if (actor->guardLevel == 0)
        actor->guardLevel = GUARD_DEFEND;
      return TURN_REPORTED;
    }
    if (BattleTurn_SetTargets(abilityId) == TURN_REPORTED)
      return TURN_REPORTED;
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(actor->inventory[request->parameter],
                            MESSAGE_ARGUMENT_ITEM);
    // An item that is kept: weapons, shields, gloves and armlets, the Psynergy
    // items, rings and ITEM_TYPE_UNUSED are raised
    // ("raises"); anything else is used.
    if (item->useType == ITEM_USE_MAY_BREAK || item->useType == ITEM_USE_FREE) {
      switch (item->type) {
      case ITEM_TYPE_WEAPON:
      case ITEM_TYPE_SHIELD:
      case ITEM_TYPE_PSYNERGY:
      case ITEM_TYPE_UNUSED:
      case ITEM_TYPE_RING:
        BattleMessage_ShowAndWaitFar(MsgActorRaisesItem);
        break;
      default:
        BattleMessage_ShowAndWaitFar(MsgActorUsesBattleItem);
        break;
      }
    } else {
      BattleMessage_ShowAndWaitFar(MsgActorUsesBattleItem);
    }
  } break;

  case COMMAND_DEFEND:
  case COMMAND_TOTAL_DEFENSE:
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgActorDefends);
    return TURN_REPORTED;

  case COMMAND_NONE:
    return TURN_NO_ACTION;

  case COMMAND_ENEMY_ABILITY: {
    s32 message;

    abilityId = request->parameter;
    if (BattleTurn_SetTargets(abilityId) == TURN_REPORTED)
      return TURN_REPORTED;
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
    if ((Ability_Get(abilityId)->flags & ABILITY_FORMULA_MASK) ==
        ABILITY_FORMULA_ENEMY_DAMAGE)
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
    BattleMessage_ShowAndWaitFar(message);
  } break;

  case COMMAND_DJINN:
    // A Djinni not set: one ready again is set, with its own short scene
    // played through the playback queue, and nothing else happens
    // (TURN_NO_ACTION); one still recovering says so (TURN_REPORTED). A set
    // Djinni is unleashed below.
    abilityId = Djinn_GetAbilityIdFar(DJINN_ELEMENT(request->parameter),
                                      DJINN_INDEX(request->parameter));
    if (!Djinn_IsSetFar(request->unitId, DJINN_ELEMENT(request->parameter),
                        DJINN_INDEX(request->parameter))) {
      if (Djinn_CanSetFar(request->unitId, DJINN_ELEMENT(request->parameter),
                          DJINN_INDEX(request->parameter))) {
        Ability_Get(abilityId);
        BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
        Djinn_SetFar(request->unitId, DJINN_ELEMENT(request->parameter),
                     DJINN_INDEX(request->parameter));
        DjinnRecovery_RemoveFar(request->unitId,
                                DJINN_ELEMENT(request->parameter),
                                DJINN_INDEX(request->parameter));
        Unit_CalculateFar(request->unitId);
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(30);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, request->unitId);
        BattlePlayback_Push(
            PLAYBACK_ABILITY_NAME,
            DJINN_ELEMENT(request->parameter) * DJINN_PER_ELEMENT +
                DJINN_INDEX(request->parameter) + ABILITY_DJINNI_FIRST);
        BattlePlayback_Push(PLAYBACK_SOUND, SE_175);
        BattlePlayback_Push(PLAYBACK_DRAW_STATUS, 0);
        BattlePlayback_Push(PLAYBACK_MESSAGE, MsgDjinnSet);
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, request->unitId);
        Sound_PlaySongFar(SE_212);
        ObjectDispatch_SelectAnimationFar(
            BattleObject_Get(request->unitId)->object, POSE_CAST);
        ObjectDispatch_SetAnimationSpeedFar(
            (struct DispatchObject *)BattleObject_Get(request->unitId)->object,
            32);
        ElementGlow_Play(request->unitId, DJINN_ELEMENT(request->parameter),
                         GLOW_BURST, 0);
        BattlePlayback_WaitForReady();
        return TURN_NO_ACTION;
      }
      // Recovering.
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      Sound_PlaySongFar(SE_MENU_ERROR);
      BattleMessage_ShowAndWaitFar(MsgDjinnInRecovery);
      WaitFrames(60);
      return TURN_REPORTED;
    }
    // Unleash the set Djinni; it goes on standby.
    {
      struct AbilityDefinition *ability;

      if (BattleTurn_SetTargets(abilityId) == TURN_REPORTED)
        return TURN_REPORTED;
      DjinnRecovery_AddStandbyFar(request->unitId,
                                  DJINN_ELEMENT(request->parameter),
                                  DJINN_INDEX(request->parameter));
      ability = Ability_Get(abilityId);
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      BattleMessage_ShowAndWaitFar(MsgActorUnleashesDjinn);
      plan->element = ability->element;
    }
    break;

  case COMMAND_SUMMON: {
    const struct SummonDefinition *summon;
    struct DjinnRecoveryList *list;
    s32 side;
    u8 counts[ELEMENT_COUNT];
    /* FAKEMATCH(dead): an unused array for the frame's 8 bytes below counts;
     * without it, 34 of 1393 instructions differ in tbs-en. */
    UNUSED u8 unused[8];
    s32 index;

    // Check whether enough Djinn of each element are on standby; the loop
    // stops at the first element short (index below 4), and leaves counts
    // holding what each element gives.
    summon = SummonDefinition_Get(request->parameter);
    BattleDjinn_CountStandby(request->unitId, counts);
    side = DJINN_RECOVERY_PARTY;
    if ((u16)request->unitId > ALLY_MAX)
      side = DJINN_RECOVERY_OPPONENT;
    list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
    for (index = 0;
         index < ELEMENT_COUNT && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->abilityId;
    if (BattleTurn_SetTargets(abilityId) == TURN_REPORTED)
      return TURN_REPORTED;
    if (index != ELEMENT_COUNT) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      BattleMessage_ShowAndWaitFar(MsgSummonLacksDjinn);
      return TURN_REPORTED;
    }
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
    BattleMessage_ShowAndWaitFar(MsgActorSummons);
    // Mark the standby Djinn being used DJINN_RECOVERY_SUMMONED, the first
    // found of each element, from units in the formation.
    for (index = 0; index != list->count; index++) {
      struct DjinnRecoveryEntry *entry = &list->entries[index];

      if (entry->turns == DJINN_RECOVERY_AFTER_BATTLE &&
          BattleUnit_IsListed(entry->unitId) && counts[entry->element] != 0) {
        entry->turns = DJINN_RECOVERY_SUMMONED;
        counts[entry->element]--;
      }
    }
  } break;
  }

  //----------------------------------------------------------------------
  // Build a normal attack.
  //----------------------------------------------------------------------
  if (abilityId == ABILITY_ATTACK) {
    struct Unit *target = Unit_GetFar(plan->targetIds[0]);

    plan->abilityId = ABILITY_ATTACK;
    plan->element = Item_GetEquippedElementFar(request->unitId);
    plan->staging = STAGING_ATTACK;
    // An enemy's attack animation comes from its class; a party member's is
    // animation 1 for Isaac, Garet and Jenna and 4 for Ivan and Mia, any other
    // none. Each is played by id range (ANIM_BY_ID_RANGE).
    if (actor->classId == CLASS_NONE) {
      plan->animation =
          BattleSprite_GetAttackAnim(actor->id) | ANIM_BY_ID_RANGE;
    } else {
      plan->animation = 0;
      switch (actor->id) {
      case ISAAC:
        plan->animation = ANIM_BY_ID_RANGE | 1;
        break;
      case GARET:
        plan->animation = ANIM_BY_ID_RANGE | 1;
        break;
      case IVAN:
        plan->animation = ANIM_BY_ID_RANGE | 4;
        break;
      case MIA:
        plan->animation = ANIM_BY_ID_RANGE | 4;
        break;
      case JENNA:
        plan->animation = ANIM_BY_ID_RANGE | 1;
        break;
      }
    }
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar(MsgActorAttacks);
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
      if (actor->delusion != 0 && (Battle_RandomFar() & 255) <= 152)
        plan->targetMultiplier[0] = 0;
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetMultiplier[0] = 0;
    } while (0);
    if (GameFlag_TestFar(FLAG_DJINN_LESSON))
      plan->targetMultiplier[0] = 0;
    // Critical hit: one chance in 32, or else the equipment's unleash bonus
    // in 200ths, against a 16-bit roll.
    if (target->hp != 0) {
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetCritical[0] = CRITICAL_HIT;
      else if ((Equipment_GetUnleashRateBonusFar(actor) << 16) / 200 >
               (Battle_RandomFar() & 0xffff))
        plan->targetCritical[0] = CRITICAL_HIT;
    }
  }

  //----------------------------------------------------------------------
  // Build an ability.
  //----------------------------------------------------------------------
  else {
    struct AbilityDefinition *ability = Ability_Get(abilityId);

    plan->element = ability->element;
    plan->animation = 0;
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
      if ((Battle_RandomFar() & 255) < chance) {
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
      if ((Battle_RandomFar() & mask) == 0) {
        s32 index;

        for (index = 0; index < plan->targetCount; index++)
          plan->targetCritical[index] = CRITICAL_STRONG;
      }
    } else if (abilityId == ABILITY_ANNIHILATION) {
      // Annihilation rolls its hit now, at full rate whatever the distance,
      // so that the animation can show it (the variant below).
      s32 index;

      for (index = 0; index < plan->targetCount; index++)
        plan->targetHit[index] =
            Battle_RollEffectSuccessFar(request->unitId, plan->targetIds[index],
                                        ability->element, ability->effect, 100);
    }
    // How it is staged: the ability's row of ability_anims.tsv, its variant
    // raised to the first target's multiplier less one (a double or triple
    // blow); then the staging, from ability_staging.tsv where it gives one. The
    // two tables end at ABILITY_THRASH and ABILITY_SEVERE_BLOW.
    if (abilityId >= 0 && abilityId <= ABILITY_THRASH) {
      plan->animation = gAbilityAnimations[abilityId];
      if (plan->targetMultiplier[0] > 1)
        plan->animation = gAbilityAnimations[abilityId] +
                          (plan->targetMultiplier[0] << ANIM_VARIANT_SHIFT) -
                          (1 << ANIM_VARIANT_SHIFT);
    }
    if ((abilityId >= 0 && abilityId <= ABILITY_SEVERE_BLOW) &&
        gAbilityStaging[abilityId] != 0)
      plan->staging = gAbilityStaging[abilityId];
    else if (Ability_IsPsynergy(abilityId))
      plan->staging = STAGING_CAST;
    else if (plan->animation != 0) {
      if (actor->classId == CLASS_NONE)
        plan->staging = STAGING_ENEMY_CAST;
      else
        plan->staging = STAGING_CAST;
    } else
      plan->staging = STAGING_MESSAGES;
    if (Battle_IsReviveEffectFar(ability->effect))
      plan->animation |= ANIM_ON_DOWNED;
    // When Annihilation's instant death lands on its first target, the
    // variant becomes 1: the burst then shows its second picture and plays a
    // sound (BattleAnim_RunAnnihilation).
    if (abilityId == ABILITY_ANNIHILATION && plan->targetHit[0] != 0)
      plan->animation |= 1 << ANIM_VARIANT_SHIFT;
  }
  // An item is staged as an item, unless its ability takes the whole screen.
  if (request->command == COMMAND_ITEM && plan->staging != STAGING_SUMMON &&
      plan->staging != STAGING_FULL_SCREEN)
    plan->staging = STAGING_ITEM;
  plan->command = request->command;
  return 0;
}
