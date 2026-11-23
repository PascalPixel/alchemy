/// @file
/// An action announced and built for its targets, BattleActionRun_Build, with
/// whether a unit is in its side's list and how many Djinn a side has on
/// standby.
// Part of battle.c.

#include "battle_internal.h"
#include "flags.h"
#include "sounds.h"
#include "field/field_sequence.h"
#include "field/flag_call.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "battle/presentation.h"

/// Returns 1 when the unit is in its side's list of units in the battle
/// (BattleUnit_ListIds), 0 otherwise.
u32 BattleUnit_IsListed(u32 unitId) {
  u16 list[8];
  s32 side;
  s32 unitCount;
  s32 unitIndex;

  side = BATTLE_SIDE_MASK_PARTY;
  if (unitId > ALLY_MAX)
    side = BATTLE_SIDE_MASK_ENEMIES;
  unitCount = BattleUnit_ListIds(side, list);

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    if (list[unitIndex] == unitId)
      break;
  }

  return unitIndex != unitCount;
}

/// Counts the standby Djinn on unitId's side, and each element's count into
/// counts when it is not null.
s32 BattleDjinn_CountStandby(u32 unitId, u8 *counts) {
  u16 unitIds[8];
  struct DjinnRecoveryList *list;
  s32 unitCount;
  s32 standbyCount;
  s32 recoveryIndex;
  s32 i;
  s32 sideMask;
  s32 enemySide;

  standbyCount = 0;
  sideMask = BATTLE_SIDE_MASK_PARTY;
  if (unitId > ALLY_MAX)
    sideMask = BATTLE_SIDE_MASK_ENEMIES;
  unitCount = BattleUnit_ListIds(sideMask, unitIds);
  enemySide = DJINN_RECOVERY_PARTY;
  if (unitId > ALLY_MAX)
    enemySide = DJINN_RECOVERY_OPPONENT;
  list = &Djinn_GetRecoveryTableFar(enemySide)->list;
  if (counts != 0)
    for (i = 0; i < ELEMENT_COUNT; i++)
      counts[i] = 0;
  for (recoveryIndex = 0; recoveryIndex != list->count; recoveryIndex++) {
    if (list->entries[recoveryIndex].turns == DJINN_RECOVERY_AFTER_BATTLE) {
      for (i = 0; i < unitCount; i++)
        if (unitIds[i] == list->entries[recoveryIndex].unitId)
          break;
      if (i != unitCount) {
        if (counts != 0)
          counts[list->entries[recoveryIndex].element]++;
        standbyCount++;
      }
    }
  }
  return standbyCount;
}

/// A range that reaches every place of a side from any of them: Dazzle's.
#define RANGE_WHOLE_SIDE 11

/// The chances, out of 256, an action rolls against the battle's random byte.
enum ActionChance {
  CHANCE_DELUDED_MISS = 153,    ///< a deluded attacker misses
  CHANCE_BLOW_LIKELY = 153,     ///< a likely double or triple blow lands
  CHANCE_BLOW_RARE = 32,        ///< a rare one
  CHANCE_BLOW = 64,             ///< any other
  CHANCE_VENGEANCE_EXTRA = 179, ///< Vengeance strikes one more time or two
  CHANCE_VENGEANCE_TWICE = 128  ///< of those, two more
};

/// Battle_RollEffectSuccess's successScale at full strength, in percent.
#define EFFECT_CHANCE_FULL 100

/// Announces the action and builds it for its targets, paying its PP or Djinn.
/// Returns 0 to stage it, ACTION_REPORTED when it failed with its message
/// shown, or ACTION_NOTHING_TO_SHOW.
s32 BattleActionRun_Build(struct BattleActionRecord *request,
                          struct BattlePlan *plan) {
  struct Unit *actor = Unit_Get(request->unitId);
  struct BattleWork *battle = (struct BattleWork *)Ram_HeapSlots->battleWork;
  s32 selectedTargetId;
  s32 abilityId;

  /// Fills the action's targets: the aimed unit, or the places within its
  /// range, every place for Dazzle; with none, the unit defends.
  s32 BattleActionRun_SetTargets(s32 abilityId) {
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
    s32 pass;

    if (Battle_IsReviveEffectFar(ability->effect))
      includeDowned = 1;
    switch (mode) {
    case ABILITY_TARGET_ONE:
      plan->targetCount = 1;
      plan->targetOffset[0] = mode;
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
      // The places within range of the aimed one, on its side.
      targetCount = 0;
      range = request->range;
      for (pass = 0; pass <= 1; pass++) {
        for (place = 0; battle->partyUnits[place] != BATTLE_UNIT_LIST_END;
             place++)
          ;
        partyCount = place;
        for (place = 0; battle->enemyIds[place] != BATTLE_UNIT_LIST_END;
             place++)
          ;
        enemyCount = place;
        aimedPlace = request->target & PLACE_INDEX_MASK;
        {
          s32 first = aimedPlace - range + 1;
          s32 last = aimedPlace + range - 1;

          for (place = first; place <= last; place++) {
            if (place < 0)
              continue;
            if (request->target & PLACE_ENEMY_SIDE) {
              if (place >= enemyCount)
                continue;
              targetId = battle->enemyIds[place];
              if (targetId == BATTLE_UNIT_REMOVED)
                continue;
              if (!includeDowned && Unit_Get(targetId)->hp == 0)
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
              if (!includeDowned && Unit_Get(targetId)->hp == 0)
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
          BattleMessage_ShowAndWaitFar((s32)&MsgActorDefends);
          if (actor->defendLevel == 0)
            actor->defendLevel = DEFEND_NORMAL;
          return ACTION_REPORTED;
        }
        if (ability->effect != ABILITY_EFFECT_DAZZLE)
          break;
        range = RANGE_WHOLE_SIDE;
      }
      break;
    }
    return 0;
  }

  // A fresh action run, and the debug defeat.
  selectedTargetId = Battle_GetUnitAtPlace(request->target);
  BattlePlayback_Reset();
  // The word after the plan in BattleWork is skipPlayback.
  *(s32 *)(plan + 1) = 0;
  plan->hauntDamage = 0;
  plan->unitId = request->unitId;
  plan->targetCount = 0;
  plan->battleEffect = 0;
  plan->failure = ACTION_FAILURE_NONE;
  plan->partnerId = request->unitId;
  plan->element = ELEMENT_NONE;
  MessageArgument_ClearAllFar();
  if (actor->hp == 0)
    return ACTION_NOTHING_TO_SHOW;

  // Debug: hold R to defeat all enemies, or R and Select the whole party.
  if (gDebugMode && GameFlag_Get(FLAG_DEBUG_BATTLE_KEYS) &&
      (gInput.held & KEY_R)) {
    s32 enemy = 1;
    s32 index;
    s32 id;

    if (gInput.held & KEY_SELECT)
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
      if (Unit_AddHpFar(id, DEFEAT_HP_CHANGE) == 0) {
        BattlePlayback_Push(PLAYBACK_HIT, id);
        BattlePlayback_Push(PLAYBACK_DEFEAT, id);
      }
    }
    BattlePlayback_RunAll();
    return ACTION_NOTHING_TO_SHOW;
  }

  // What stops the unit, unless it strikes back with the Djinn Counter:
  // Ground, sleep, stun, a bound unit's bad luck.
  MessageArgument_ClearAllFar();
  if (request->command != COMMAND_DJINN_COUNTER) {
    if (actor->immobile) {
      actor->immobile = 0;
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgUnableToMove);
      return ACTION_REPORTED;
    }
    if (actor->sleep) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgIsAsleep);
      return ACTION_REPORTED;
    }
    if (actor->stun) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgIsParalyzed);
      return ACTION_REPORTED;
    }
    // A bound unit loses one action in four, except Defend.
    if ((actor->curse & CURSE_BOUND) && request->command != COMMAND_DEFEND &&
        (Battle_RandomFar() & 3) == 0) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgIsBound);
      return ACTION_REPORTED;
    }
  }
  if (request->command == COMMAND_NONE)
    return ACTION_NOTHING_TO_SHOW;
  abilityId = ABILITY_ATTACK;
  {
    s32 index;

    for (index = 0; index < BATTLE_TARGET_CAPACITY; index++)
      plan->targetCritical[index] = 0;
    for (index = 0; index < BATTLE_TARGET_CAPACITY; index++)
      plan->targetHit[index] = HIT_NOT_ROLLED;
  }

  // The announcement, the ability id and the targets, by command.
  switch (request->command) {
  case COMMAND_FLEE:
    if ((u16)request->unitId <= ALLY_MAX) {
      BattleMessage_ShowAndWaitFar((s32)&MsgPartyFlees);
    } else {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgActorRuns);
    }
    BattleMessage_WaitForAdvance();
    plan->staging = STAGING_FLEE;
    return 0;

  case COMMAND_ATTACK:
    // A weapon unleash replaces the action with its ability.
    abilityId = RollWeaponUnleashFar(actor);
    if (BattleActionRun_SetTargets(abilityId) == ACTION_REPORTED)
      return ACTION_REPORTED;
    if (abilityId != ABILITY_ATTACK) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(Inventory_GetEquippedItemFar(actor, EQUIP_WEAPON),
                              MESSAGE_ARGUMENT_ITEM);
      BattleMessage_ShowAndWaitFar((s32)&MsgWeaponHowls);
      BattleMessage_WaitForTyping();
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      BattleMessage_ShowAndWaitFar((s32)(MsgWeaponHowls + 1));
    }
    break;

  case COMMAND_DJINN_COUNTER:
    abilityId = ABILITY_DJINN_COUNTER;
    // Poseidon answers with a plain attack.
    if ((u16)(actor->enemyId - ENEMY_POSEIDON) <=
        ENEMY_POSEIDON_3 - ENEMY_POSEIDON)
      abilityId = ABILITY_ATTACK;
    if (selectedTargetId == BATTLE_UNIT_LIST_END)
      return ACTION_NOTHING_TO_SHOW;
    plan->targetCount = 1;
    plan->targetOffset[0] = 0;
    plan->targetIds[0] = selectedTargetId;
    plan->targetMultiplier[0] = 1;
    break;

  case COMMAND_PSYNERGY: {
    struct AbilityDefinition *ability;
    s32 canCast = 1;

    abilityId = request->parameter;
    ability = Ability_Get(abilityId);
    if (BattleActionRun_SetTargets(abilityId) == ACTION_REPORTED)
      return ACTION_REPORTED;
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
    switch (abilityId) {
    case ABILITY_WILD_WOLF ... ABILITY_PHOENIX:
      BattleMessage_ShowAndWaitFar((s32)&MsgActorCastsSpecial);
      break;
    case ABILITY_FLAME_CARD ... ABILITY_DEATH_CARD:
      BattleMessage_ShowAndWaitFar((s32)&MsgActorCastsDjinni);
      break;
    default:
      BattleMessage_ShowAndWaitFar((s32)&MsgActorCasts);
      break;
    }
    // Too little PP or a seal fails the cast; only a cast that goes ahead pays.
    if (actor->pp < ability->ppCost) {
      plan->failure = ACTION_FAILURE_NO_PP;
      canCast = 0;
    }
    if (actor->psySeal) {
      plan->failure = ACTION_FAILURE_SEALED;
      canCast = 0;
    }
    if (canCast) {
      plan->failure = ACTION_FAILURE_NONE;
      actor->pp -= ability->ppCost;
      Unit_RecalculateGaugesFar(request->unitId);
      if (actor->pp < 0)
        actor->pp = 0;
      if (actor->pp > actor->ppMax)
        actor->pp = actor->ppMax;
    }
  } break;

  case COMMAND_ITEM: {
    struct ItemDefinition *item;

    // A slot of -1: an earlier command this turn used the item up
    // (BattleStaging_RunItem).
    if (request->parameter < 0) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgItemAlreadyUsed);
      return ACTION_REPORTED;
    }
    // An item with no ability, or a broken one, and the user defends.
    item = Item_GetDirectFar(actor->inventory[request->parameter]);
    abilityId = item->abilityId;
    if (abilityId == 0 ||
        (actor->inventory[request->parameter] & INVENTORY_BROKEN)) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgActorDefends);
      if (actor->defendLevel == 0)
        actor->defendLevel = DEFEND_NORMAL;
      return ACTION_REPORTED;
    }
    if (BattleActionRun_SetTargets(abilityId) == ACTION_REPORTED)
      return ACTION_REPORTED;
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(actor->inventory[request->parameter],
                            MESSAGE_ARGUMENT_ITEM);
    // An item that is kept is raised ("raises"); anything else is used.
    if (item->useType == ITEM_USE_MAY_BREAK || item->useType == ITEM_USE_FREE) {
      switch (item->type) {
      case ITEM_TYPE_WEAPON:
      case ITEM_TYPE_SHIELD:
      case ITEM_TYPE_PSYNERGY:
      case ITEM_TYPE_TRIDENT:
      case ITEM_TYPE_RING:
        BattleMessage_ShowAndWaitFar((s32)&MsgActorRaisesItem);
        break;
      default:
        BattleMessage_ShowAndWaitFar((s32)&MsgActorUsesBattleItem);
        break;
      }
    } else {
      BattleMessage_ShowAndWaitFar((s32)&MsgActorUsesBattleItem);
    }
  } break;

  case COMMAND_DEFEND:
  case COMMAND_TOTAL_DEFENSE:
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)&MsgActorDefends);
    return ACTION_REPORTED;

  case COMMAND_NONE:
    return ACTION_NOTHING_TO_SHOW;

  case COMMAND_ABILITY: {
    s32 message;

    abilityId = request->parameter;
    if (BattleActionRun_SetTargets(abilityId) == ACTION_REPORTED)
      return ACTION_REPORTED;
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
    if ((Ability_Get(abilityId)->flags & ABILITY_FORMULA_MASK) ==
        ABILITY_FORMULA_ENEMY_DAMAGE)
      message = (s32)&MsgActorUnleashesAbility;
    else
      message = (s32)&MsgActorUsesAbility;
    // Some enemy abilities have their own wording.
    switch (abilityId) {
    case ABILITY_SONIC_WAVE:
    case ABILITY_SONIC_WAVE_STRONG:
      message = (s32)&MsgEmitsUltrasonicWaves;
      break;
    case ABILITY_HEAT_MIRAGE:
      message = (s32)&MsgActorCasts;
      break;
    case ABILITY_THREATEN:
      message = (s32)&MsgGlowersFerociously;
      break;
    case ABILITY_TREMBLE:
      message = (s32)&MsgIsIntimidated;
      break;
    case ABILITY_CONTAIN:
      message = (s32)&MsgBalefulGaze;
      break;
    case ABILITY_WORMS:
      message = (s32)&MsgEatsWorms;
      break;
    case ABILITY_SHRIEK:
    case ABILITY_BANSHEE_HOWL:
    case ABILITY_CRAZY_VOICE:
    case ABILITY_WAR_CRY:
    case ABILITY_WICKED_HOWL:
      message = (s32)&MsgLetsOutAbility;
      break;
    case ABILITY_WING_BEAT:
    case ABILITY_WING_FLUTTER:
    case ABILITY_WING_STROKE:
      message = (s32)&MsgActorUsesAbility;
      break;
    case ABILITY_ROTTEN_BLOOD:
      message = (s32)&MsgEnemyUnleashesAbility;
      break;
    case ABILITY_HEARTRENDER:
      message = (s32)&MsgGlowersMiserably;
      break;
    case ABILITY_DECOMPOSE:
      message = (s32)&MsgSmellOfDecay;
      break;
    case ABILITY_BERSERK:
      message = (s32)&MsgFuriousRage;
      break;
    case ABILITY_SPEED_SURGE:
      message = (s32)&MsgAttemptsToDivide;
      break;
    case ABILITY_ALLY_SEARCH:
      message = (s32)&MsgLooksForAllies;
      break;
    case ABILITY_BATTLE_CRY:
      message = (s32)&MsgLooksForHelp;
      break;
    case ABILITY_PRESSURE:
      message = (s32)&MsgCannotMove;
      break;
    }
    BattleMessage_ShowAndWaitFar(message);
  } break;

  case COMMAND_DJINN:
    // A Djinni on standby that has recovered is set again, and nothing else
    // happens; one still recovering says so. A set Djinni is unleashed.
    abilityId = Djinn_GetAbilityIdFar(DJINN_COMMAND_ELEMENT(request->parameter),
                                      DJINN_COMMAND_INDEX(request->parameter));
    if (!Djinn_IsSetFar(request->unitId,
                        DJINN_COMMAND_ELEMENT(request->parameter),
                        DJINN_COMMAND_INDEX(request->parameter))) {
      if (Djinn_CanSetFar(request->unitId,
                          DJINN_COMMAND_ELEMENT(request->parameter),
                          DJINN_COMMAND_INDEX(request->parameter))) {
        Ability_Get(abilityId);
        BattleHighlight_SetModes(0, HIGHLIGHT_OFF);
        Djinn_SetFar(request->unitId, DJINN_COMMAND_ELEMENT(request->parameter),
                     DJINN_COMMAND_INDEX(request->parameter));
        DjinnRecovery_RemoveFar(request->unitId,
                                DJINN_COMMAND_ELEMENT(request->parameter),
                                DJINN_COMMAND_INDEX(request->parameter));
        Unit_CalculateFar(request->unitId);
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(30);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, request->unitId);
        BattlePlayback_Push(
            PLAYBACK_ABILITY_NAME,
            DJINN_COMMAND_ELEMENT(request->parameter) * DJINN_PER_ELEMENT +
                DJINN_COMMAND_INDEX(request->parameter) + ABILITY_DJINNI_FIRST);
        BattlePlayback_Push(PLAYBACK_SOUND, SE_NOTE_HIGH);
        BattlePlayback_Push(PLAYBACK_DRAW_STATUS, 0);
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgDjinnIsSet);
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, request->unitId);
        Sound_PlayFar(SE_POWER_UP);
        Actor_SelectAnimationFar(BattleObject_Get(request->unitId)->object,
                                 POSE_CAST);
        Actor_SetAnimationSpeedFar(
            (struct Actor *)BattleObject_Get(request->unitId)->object,
            ANIM_STEP_NORMAL * 2);
        ElementGlow_Play(request->unitId,
                         DJINN_COMMAND_ELEMENT(request->parameter), GLOW_BURST,
                         0);
        BattlePlayback_WaitForReady();
        return ACTION_NOTHING_TO_SHOW;
      }
      // Recovering.
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      Sound_PlayFar(SE_MENU_ERROR);
      BattleMessage_ShowAndWaitFar((s32)&MsgDjinnInRecovery);
      wait(60);
      return ACTION_REPORTED;
    }
    // Unleash the set Djinni; it goes on standby.
    {
      struct AbilityDefinition *ability;

      if (BattleActionRun_SetTargets(abilityId) == ACTION_REPORTED)
        return ACTION_REPORTED;
      DjinnRecovery_AddStandbyFar(request->unitId,
                                  DJINN_COMMAND_ELEMENT(request->parameter),
                                  DJINN_COMMAND_INDEX(request->parameter));
      ability = Ability_Get(abilityId);
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      BattleMessage_ShowAndWaitFar((s32)&MsgActorUnleashesDjinn);
      plan->element = ability->element;
    }
    break;

  case COMMAND_SUMMON: {
    const struct SummonDefinition *summon;
    struct DjinnRecoveryList *list;
    s32 side;
    u8 counts[ELEMENT_COUNT];
    s32 index;

    // Enough Djinn of each element on standby? A link battle counts the
    // summoner's side; any other counts the party's.
    summon = SummonDefinition_Get(request->parameter);
    BattleDjinn_CountStandby(
        ((struct BattleWork *)Ram_HeapSlots->battleWork)->isLink
            ? request->unitId
            : 0,
        counts);
    side = DJINN_RECOVERY_PARTY;
    if ((u16)request->unitId > ALLY_MAX &&
        ((struct BattleWork *)Ram_HeapSlots->battleWork)->isLink)
      side = DJINN_RECOVERY_OPPONENT;
    list = &Djinn_GetRecoveryTableFar(side)->list;
    for (index = 0;
         index < ELEMENT_COUNT && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->abilityId;
    if (BattleActionRun_SetTargets(abilityId) == ACTION_REPORTED)
      return ACTION_REPORTED;
    if (index != ELEMENT_COUNT) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      BattleMessage_ShowAndWaitFar((s32)&MsgSummonLacksDjinn);
      return ACTION_REPORTED;
    }
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
    BattleMessage_ShowAndWaitFar((s32)&MsgActorSummons);
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
    // Daedalus strikes again at the turn's end (BattleStaging_PlanDaedalus).
    if (abilityId == ABILITY_DAEDALUS) {
      if ((u16)request->unitId > ALLY_MAX)
        battle->nextDaedalusFromEnemies |= 1 << battle->nextDaedalusStrikes;
      battle->nextDaedalusStrikes++;
    }
  } break;
  }

  if (abilityId == ABILITY_ATTACK) {
    struct Unit *target = Unit_Get(plan->targetIds[0]);

    plan->abilityId = ABILITY_ATTACK;
    plan->element = Item_GetEquippedElementFar(request->unitId);
    plan->staging = STAGING_ATTACK;
    // An enemy's attack effect comes from its class; a party member's from who
    // it is.
    if (actor->classId == CLASS_NONE) {
      plan->battleEffect =
          BattleSprite_GetAttackEffect(actor->enemyId) | EFFECT_BY_ID_RANGE;
    } else {
      plan->battleEffect = 0;
      plan->battleEffect = BattleUnit_GetPartyAttackEffect(actor->enemyId);
    }
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)&MsgActorAttacks);
    // A deluded attacker misses CHANCE_DELUDED_MISS times in 256, and any
    // attack 1 in 32; a target that cannot move is never missed, and in the
    // Djinn tutorial the attack always misses.
    do {
      if (target->hp == 0 || target->sleep != 0 || target->stun != 0 ||
          target->immobile != 0 || target->charm != 0)
        break;
      if (actor->delusion != 0 &&
          (Battle_RandomFar() & 255) < CHANCE_DELUDED_MISS)
        plan->targetMultiplier[0] = 0;
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetMultiplier[0] = 0;
    } while (0);
    if (GameFlag_Get(FLAG_DJINN_LESSON))
      plan->targetMultiplier[0] = 0;
    // Critical hit: one chance in 32, or else the equipment's unleash bonus
    // in 200ths, against a 16-bit roll.
    if (target->hp != 0) {
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetCritical[0] = CRITICAL_HIT;
      else if (FIXED_ONE * Equipment_GetUnleashRateBonusFar(actor) / 200 >
               (Battle_RandomFar() & 0xffff))
        plan->targetCritical[0] = CRITICAL_HIT;
    }
  } else {
    struct AbilityDefinition *ability = Ability_Get(abilityId);
    s32 chance;
    s32 amount;

    plan->abilityId = abilityId;
    plan->element = ability->element;
    plan->battleEffect = 0;
    if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_LIKELY ||
        ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_RARE ||
        ability->effect == ABILITY_EFFECT_DOUBLE_BLOW ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_RARE ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_LIKELY ||
        ability->effect == ABILITY_EFFECT_VENGEANCE) {
      s32 index;

      // Vengeance strikes one or two more times, a double or triple blow one
      // or two, each at its chance: the multiplier on every target rises.
      if (ability->effect == ABILITY_EFFECT_VENGEANCE) {
        s32 roll = Battle_RandomFar() & 255;

        if (roll < CHANCE_VENGEANCE_EXTRA) {
          amount = 1;
          if (roll < CHANCE_VENGEANCE_TWICE)
            amount = 2;
          for (index = 0; index < plan->targetCount; index++)
            plan->targetMultiplier[index] += amount;
        }
      } else {
        if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_LIKELY ||
            ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_LIKELY)
          chance = CHANCE_BLOW_LIKELY;
        else if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_RARE ||
                 ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_RARE)
          chance = CHANCE_BLOW_RARE;
        else
          chance = CHANCE_BLOW;
        if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_LIKELY ||
            ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_RARE ||
            ability->effect == ABILITY_EFFECT_DOUBLE_BLOW)
          amount = 1;
        else
          amount = 2;
        if ((Battle_RandomFar() & 255) < chance) {
          for (index = 0; index < plan->targetCount; index++)
            plan->targetMultiplier[index] += amount;
        }
      }
    } else if (ability->effect >= ABILITY_EFFECT_CRITICAL_A &&
               ability->effect <= ABILITY_EFFECT_CRITICAL_E) {
      // An ability's critical hit on every target, 1 in 64 to 1 in 4.
      s32 mask;

      switch (ability->effect) {
      case ABILITY_EFFECT_CRITICAL_A:
        mask = 64 - 1;
        break;
      case ABILITY_EFFECT_CRITICAL_B:
        mask = 32 - 1;
        break;
      case ABILITY_EFFECT_CRITICAL_C:
        mask = 16 - 1;
        break;
      case ABILITY_EFFECT_CRITICAL_D:
        mask = 8 - 1;
        break;
      case ABILITY_EFFECT_CRITICAL_E:
      default:
        mask = 4 - 1;
        break;
      }
      if ((Battle_RandomFar() & mask) == 0) {
        s32 index;

        for (index = 0; index < plan->targetCount; index++)
          plan->targetCritical[index] = CRITICAL_STRONG;
      }
    } else if (abilityId == ABILITY_ANNIHILATION ||
               abilityId == ABILITY_WHORL ||
               ability->effect == ABILITY_EFFECT_BLOW_AWAY) {
      // Annihilation, Whorl and Gale roll their hit now, so that the effect
      // can show it (the variant below): against foes at the falloff for each
      // target's distance, at full rate otherwise.
      s32 index;

      for (index = 0; index < plan->targetCount; index++) {
        s32 rate;

        if (ability->targetMode == ABILITY_TARGET_FOES) {
          rate =
              gActionRun_EffectChance[__builtin_abs(plan->targetOffset[index])];
        } else {
          rate = EFFECT_CHANCE_FULL;
        }
        plan->targetHit[index] = Battle_RollEffectSuccessFar(
            request->unitId, plan->targetIds[index], ability->element,
            ability->effect, rate);
      }
    }
    if (abilityId == ABILITY_MELD) {
      plan->partnerId = BattleUnit_FindStrongestAlly(plan->unitId);
    } else if (abilityId == ABILITY_MOLD) {
      s32 tries = 0;

      plan->partnerId = plan->targetIds[0];
      do {
        plan->targetIds[0] = Battle_GetUnitAtPlace(
            BattleAI_ChooseRandomTarget(plan->partnerId <= ALLY_MAX));
        if (plan->targetIds[0] != plan->partnerId)
          break;
      } while (++tries <= 9);
    }
    // How it is staged: the ability's battle effect, stronger for a double or
    // triple blow, and its staging.
    if (abilityId >= 0 && abilityId <= ABILITY_PSY_BOOST) {
      plan->battleEffect = gAbilityBattleEffects[abilityId];
      if (plan->targetMultiplier[0] > 1)
        plan->battleEffect =
            gAbilityBattleEffects[abilityId] +
            (plan->targetMultiplier[0] << EFFECT_VARIANT_SHIFT) -
            (1 << EFFECT_VARIANT_SHIFT);
    }
    if ((abilityId >= 0 && abilityId <= ABILITY_PSY_BOOST) &&
        gAbilityStaging[abilityId] != 0)
      plan->staging = gAbilityStaging[abilityId];
    else if (Ability_IsPsynergy(abilityId))
      plan->staging = STAGING_CAST;
    else if (plan->battleEffect != 0) {
      if (actor->classId == CLASS_NONE)
        plan->staging = STAGING_ENEMY_CAST;
      else
        plan->staging = STAGING_CAST;
    } else
      plan->staging = STAGING_MESSAGES;
    if (Battle_IsReviveEffectFar(ability->effect))
      plan->battleEffect |= EFFECT_ON_DOWNED;
    // When the hit rolled ahead lands on the first target, the effect plays
    // its next variant.
    if ((abilityId == ABILITY_ANNIHILATION || abilityId == ABILITY_WHORL ||
         ability->effect == ABILITY_EFFECT_BLOW_AWAY) &&
        plan->targetHit[0] != 0)
      plan->battleEffect |= 1 << EFFECT_VARIANT_SHIFT;
  }
  // An item is staged as an item, unless its ability takes the whole screen.
  if (request->command == COMMAND_ITEM && plan->staging != STAGING_SUMMON &&
      plan->staging != STAGING_FULL_SCREEN)
    plan->staging = STAGING_ITEM;
  plan->command = request->command;
  return 0;
}
