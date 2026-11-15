/// @file
/// Whether a unit stands in its side's formation and how many Djinn a side has
/// on standby, and one routine not yet understood.
// Part of battle.c.

#include "../battle_internal.h"
#include "flags.h"
#include "sounds.h"
#include "flag_call.h"
#include "gamepak/far_call/game.h"
#include "module/battle/battle.h"
#include "module/battle/presentation.h"
#include "module/battle/presentation.h"

// Battle: test whether a unit is in a battle formation.

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
  unitCount = BattleUnit_ListIds(sideMask, (u16 *)unitIds);
  enemySide = DJINN_RECOVERY_PARTY;
  if (unitId > ALLY_MAX)
    enemySide = DJINN_RECOVERY_OPPONENT;
  list = &Djinn_GetRecoveryTableFar(enemySide)->list;
  if (counts != 0)
    for (i = ELEMENT_COUNT - 1; i >= 0; i--)
      counts[i] = 0;
  recoveryIndex = 0;
  if (list->count != 0) {
    do {
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
      recoveryIndex++;
    } while (recoveryIndex != list->count);
  }
  return standbyCount;
}

/// The most targets an action holds, 14. BattlePlan's declaration counts 13 and
/// names each list's fourteenth entry apart.
#define TARGET_MAX 14

/* VIEW(presentation): routine Battle_GetUnitAtPlace as s32
 * Battle_GetUnitAtPlace(s32 tag); battle/battle has s16
 * Battle_GetUnitAtPlace(s32 slot) {; tried as the definition's s16
 * Battle_GetUnitAtPlace(s32 slot), BattleActionRun_Build moves: 4 of 1601
 * instructions in tla-ja. */
s32 Battle_GetUnitAtPlace__presentation(s32 tag) __asm__(
    "Battle_GetUnitAtPlace");
#define Battle_GetUnitAtPlace Battle_GetUnitAtPlace__presentation

/// Announces the action and builds it for its targets, paying its PP or Djinn.
/// Returns 0 to stage it, ACTION_REPORTED when it failed with its message
/// shown, or ACTION_NOTHING_TO_SHOW.
s32 BattleActionRun_Build(struct BattleActionRecord *request,
                          struct BattlePlan *plan) {
  struct Unit *actor = Unit_Get(request->unitId);
  struct ActionRunBattleWork *battle =
      (struct ActionRunBattleWork *)Ram_HeapSlots->battleWork;
  s32 selectedTargetId;
  s32 abilityId;

  s32 BattleActionRun_SetTargets(s32 abilityId) {
    struct BattleAct *ability = Ability_Get(abilityId);
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
        aimedPlace = request->target & 15;
        {
          s32 first = aimedPlace - range + 1;
          s32 last = aimedPlace + range - 1;

          for (place = first; place <= last; place++) {
            if (place < 0)
              continue;
            if (request->target & 128) {
              if (place >= enemyCount)
                continue;
              targetId = battle->enemyIds[place];
              if (targetId == BATTLE_UNIT_REMOVED)
                continue;
              if (!includeDowned &&
                  ((struct Unit *)Unit_Get(targetId))->hp == 0)
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
              if (!includeDowned &&
                  ((struct Unit *)Unit_Get(targetId))->hp == 0)
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
        if (ability->effect != 75)
          break;
        range = 11;
      }
      break;
    }
    return 0;
  }

  selectedTargetId = Battle_GetUnitAtPlace(request->target);
  BattlePlayback_Reset();
  ((s32 *)plan)[25] = 0;
  plan->hauntDamage = 0;
  plan->unitId = request->unitId;
  plan->targetCount = 0;
  plan->battleEffect = 0;
  plan->failure = ACTION_FAILURE_NONE;
  plan->actorId2 = request->unitId;
  plan->element = ELEMENT_NONE;
  MessageArgument_ClearAllFar();
  if (actor->hp == 0)
    return ACTION_NOTHING_TO_SHOW;

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
      if (Unit_AddHpFar(id, 0xc0000000) == 0) {
        BattlePlayback_Push(PLAYBACK_HIT, id);
        BattlePlayback_Push(PLAYBACK_DEFEAT, id);
      }
    }
    BattlePlayback_RunAll();
    return ACTION_NOTHING_TO_SHOW;
  }

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

    for (index = 0; index < TARGET_MAX; index++)
      plan->targetCritical[index] = 0;
    for (index = 0; index < TARGET_MAX; index++)
      plan->targetHit[index] = HIT_NOT_ROLLED;
  }

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
    if ((u16)(actor->enemyId - 101) <= 2)
      abilityId = 1;
    if (selectedTargetId == BATTLE_UNIT_LIST_END)
      return ACTION_NOTHING_TO_SHOW;
    plan->targetCount = 1;
    plan->targetOffset[0] = 0;
    plan->targetIds[0] = selectedTargetId;
    plan->targetMultiplier[0] = 1;
    break;

  case COMMAND_PSYNERGY: {
    struct BattleAct *ability;
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

    if (request->parameter < 0) {
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      BattleMessage_ShowAndWaitFar((s32)&MsgItemAlreadyUsed);
      return ACTION_REPORTED;
    }
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
    if (item->useType == 2 || item->useType == 0) {
      switch (item->type) {
      case 1:
      case 3:
      case 6:
      case 7:
      case 8:
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
  case COMMAND_DEFEND2:
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
    switch (abilityId) {
    case ABILITY_SONIC_WAVE:
    case ABILITY_SONIC_WAVE_STRONG:
      message = (s32)&MsgEnemyAbility0d53;
      break;
    case ABILITY_HEAT_MIRAGE:
      message = (s32)&MsgActorCasts;
      break;
    case ABILITY_THREATEN:
      message = (s32)&MsgEnemyAbility0d58;
      break;
    case ABILITY_TREMBLE:
      message = (s32)&MsgEnemyAbility0d59;
      break;
    case ABILITY_CONTAIN:
      message = (s32)&MsgEnemyAbility0d5a;
      break;
    case ABILITY_WORMS:
      message = (s32)&MsgEnemyAbility0d5b;
      break;
    case ABILITY_SHRIEK:
    case ABILITY_BANSHEE_HOWL:
    case ABILITY_CRAZY_VOICE:
    case ABILITY_WAR_CRY:
    case ABILITY_WICKED_HOWL:
      message = (s32)&MsgEnemyAbility0d5c;
      break;
    case ABILITY_WING_BEAT:
    case ABILITY_WING_FLUTTER:
    case ABILITY_WING_STROKE:
      message = (s32)&MsgActorUsesAbility;
      break;
    case ABILITY_ROTTEN_BLOOD:
      message = (s32)&MsgEnemyAbility0d5d;
      break;
    case ABILITY_HEARTRENDER:
      message = (s32)&MsgEnemyAbility0d5e;
      break;
    case ABILITY_DECOMPOSE:
      message = (s32)&MsgEnemyAbility0d60;
      break;
    case ABILITY_BERSERK:
      message = (s32)&MsgEnemyAbility0d5f;
      break;
    case ABILITY_SPEED_SURGE:
      message = (s32)&MsgEnemyAbility0d61;
      break;
    case ABILITY_ALLY_SEARCH:
      message = (s32)&MsgEnemyAbility0d62;
      break;
    case ABILITY_BATTLE_CRY:
      message = (s32)&MsgEnemyAbility0d63;
      break;
    case ABILITY_PRESSURE:
      message = (s32)&MsgEnemyAbility0d65;
      break;
    }
    BattleMessage_ShowAndWaitFar(message);
  } break;

  case COMMAND_DJINN:
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
        BattlePlayback_Push(PLAYBACK_ABILITY_NAME,
                            DJINN_ELEMENT(request->parameter) * 20 +
                                DJINN_INDEX(request->parameter) + 300);
        BattlePlayback_Push(PLAYBACK_SOUND, 175);
        BattlePlayback_Push(PLAYBACK_DRAW_STATUS, 0);
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgDjinnIsSet);
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, request->unitId);
        Sound_PlayFar(SE_POWER_UP);
        Actor_SelectAnimationFar(BattleObject_Get(request->unitId)->object, 3);
        Actor_SetAnimationSpeedFar(
            (struct Actor *)BattleObject_Get(request->unitId)->object, 32);
        ElementGlow_Play(request->unitId, DJINN_ELEMENT(request->parameter), 3,
                         0);
        BattlePlayback_WaitForReady();
        return ACTION_NOTHING_TO_SHOW;
      }
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      Sound_PlayFar(SE_MENU_ERROR);
      BattleMessage_ShowAndWaitFar((s32)&MsgDjinnInRecovery);
      wait(60);
      return ACTION_REPORTED;
    }
    {
      struct BattleAct *ability;

      if (BattleActionRun_SetTargets(abilityId) == ACTION_REPORTED)
        return ACTION_REPORTED;
      DjinnRecovery_AddStandbyFar(request->unitId,
                                  DJINN_ELEMENT(request->parameter),
                                  DJINN_INDEX(request->parameter));
      ability = Ability_Get(abilityId);
      MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(abilityId, MESSAGE_ARGUMENT_ABILITY);
      BattleMessage_ShowAndWaitFar((s32)&MsgActorUnleashesDjinn);
      plan->element = ability->element;
    }
    break;

  case COMMAND_SUMMON: {
    const struct SummonDefinition *summon;
    struct DjinnRecoveryListDraft *list;
    s32 side;
    u8 counts[4];
    s32 index;

    summon = SummonDefinition_Get(request->parameter);
    BattleDjinn_CountStandby(
        ((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided
            ? request->unitId
            : 0,
        counts);
    side = 0;
    if ((u16)request->unitId > ALLY_MAX &&
        ((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided)
      side = 1;
    list = (struct DjinnRecoveryListDraft *)((u8 *)Djinn_GetRecoveryTableFar(
                                                 side) +
                                             8);
    for (index = 0;
         index < ELEMENT_COUNT && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->nameMessageId;
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
    for (index = 0; index != list->count; index++) {
      struct DjinnRecoveryEntryDraft *entry = &list->entries[index];

      if (entry->turns == DJINN_RECOVERY_AFTER_BATTLE &&
          BattleUnit_IsListed(entry->unitId) && counts[entry->element] != 0) {
        entry->turns = DJINN_RECOVERY_SUMMONED;
        counts[entry->element]--;
      }
    }
    if (abilityId == ABILITY_DAEDALUS) {
      if ((u16)request->unitId > ALLY_MAX)
        battle->summonedMask |= 1 << battle->summonCount;
      battle->summonCount++;
    }
  } break;
  }

  if (abilityId == ABILITY_ATTACK) {
    struct Unit *target = Unit_Get(plan->targetIds[0]);

    plan->abilityId = ABILITY_ATTACK;
    plan->element = Item_GetEquippedElementFar(request->unitId);
    plan->staging = STAGING_ATTACK;
    if (actor->classId == CLASS_NONE) {
      plan->battleEffect =
          BattleSprite_GetAttackEffect(actor->enemyId) | EFFECT_BY_ID_RANGE;
    } else {
      plan->battleEffect = 0;
      plan->battleEffect = BattleUnit_GetPartyAttackEffect(actor->enemyId);
    }
    MessageArgument_PushFar(request->unitId, MESSAGE_ARGUMENT_ACTOR);
    BattleMessage_ShowAndWaitFar((s32)&MsgActorAttacks);
    /* FAKEMATCH(dead): the rolls in a one-pass block left by break. */
    do {
      if (target->hp == 0 || target->sleep != 0 || target->stun != 0 ||
          target->immobile != 0 || target->charm != 0)
        break;
      if (actor->delusion != 0 && (Battle_RandomFar() & 255) <= 152)
        plan->targetMultiplier[0] = 0;
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetMultiplier[0] = 0;
    } while (0);
    if (GameFlag_Get(FLAG_DJINN_LESSON))
      plan->targetMultiplier[0] = 0;
    if (target->hp != 0) {
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetCritical[0] = CRITICAL_HIT;
      else if ((Equipment_GetUnleashRateBonusFar(actor) << 16) / 200 >
               (Battle_RandomFar() & 0xffff))
        plan->targetCritical[0] = CRITICAL_HIT;
    }
  } else {
    struct BattleAct *ability = Ability_Get(abilityId);
    s32 chance;
    s32 amount;

    plan->abilityId = abilityId;
    plan->element = ability->element;
    plan->battleEffect = 0;
    if (ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_60 ||
        ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_1_IN_8 ||
        ability->effect == ABILITY_EFFECT_DOUBLE_BLOW_1_IN_4 ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_1_IN_8 ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_1_IN_4 ||
        ability->effect == ABILITY_EFFECT_TRIPLE_BLOW_60 ||
        ability->effect == 89) {
      s32 index;

      if (ability->effect == 89) {
        s32 roll = Battle_RandomFar() & 255;

        if (roll <= 178) {
          amount = 1;
          if (roll <= 127)
            amount = 2;
          for (index = 0; index < plan->targetCount; index++)
            plan->targetMultiplier[index] += amount;
        }
      } else {
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
          for (index = 0; index < plan->targetCount; index++)
            plan->targetMultiplier[index] += amount;
        }
      }
    } else if (ability->effect >= ABILITY_EFFECT_CRITICAL_1_IN_64 &&
               ability->effect <= ABILITY_EFFECT_CRITICAL_1_IN_4) {
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
    } else if (abilityId == ABILITY_ANNIHILATION ||
               abilityId == ABILITY_WHORL || ability->effect == 82) {
      s32 index;

      for (index = 0; index < plan->targetCount; index++) {
        s32 rate;

        if (ability->targetMode == ABILITY_TARGET_FOES) {
          rate =
              gActionRun_EffectChance[__builtin_abs(plan->targetOffset[index])];
        } else {
          rate = 100;
        }
        plan->targetHit[index] = Battle_RollEffectSuccessFar(
            request->unitId, plan->targetIds[index], ability->element,
            ability->effect, rate);
      }
    }
    if (abilityId == ABILITY_MELD) {
      plan->actorId2 = BattleUnit_FindStrongestAlly(plan->unitId);
    } else if (abilityId == ABILITY_MOLD) {
      s32 tries = 0;

      plan->actorId2 = plan->targetIds[0];
      do {
        plan->targetIds[0] = Battle_GetUnitAtPlace(
            BattleAI_ChooseRandomTarget(plan->actorId2 <= ALLY_MAX));
        if (plan->targetIds[0] != plan->actorId2)
          break;
      } while (++tries <= 9);
    }
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
    if ((abilityId == ABILITY_ANNIHILATION || abilityId == ABILITY_WHORL ||
         ability->effect == 82) &&
        plan->targetHit[0] != 0)
      plan->battleEffect |= 1 << EFFECT_VARIANT_SHIFT;
  }
  if (request->command == COMMAND_ITEM && plan->staging != STAGING_SUMMON &&
      plan->staging != STAGING_FULL_SCREEN)
    plan->staging = STAGING_ITEM;
  plan->command = request->command;
  return 0;
}
