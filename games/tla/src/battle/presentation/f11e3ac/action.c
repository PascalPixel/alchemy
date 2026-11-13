/// @file
/// Whether a unit stands in its side's formation and how many Djinn a side has
/// on standby, and one routine not yet understood.
// Part of f11e3ac.c.

#include "../f11e3ac_internal.h"
#include "flag_call.h"
#include "gamepak/far_call/game.h"
#include "module/battle/battle.h"
#include "module/battle/f11e3ac.h"
#include "module/battle/presentation.h"
#include "module/tla.h"

// Battle: test whether a unit is in a battle formation.

/// Returns 1 if the unit is in its side's formation.
/// id: unit ID (0-7 allies, all others enemies).
u32 BattleParty_IsUnitListed(u32 id) {
  u16 list[8];
  s32 side;
  s32 cnt;
  s32 i;

  side = BATTLE_SIDE_PARTY;
  if (id > 7)
    side = BATTLE_SIDE_ENEMIES;
  cnt = BattleParty_ListActorIds(side, list);

  for (i = 0; i < cnt; i++) {
    if (list[i] == id)
      break;
  }

  return i != cnt;
}

s32 BattleParty_CountStandbyDjinn(u32 arg0, u8 *counts) {
  u16 values[8];
  struct PlacementList *list;
  s32 total;
  s32 found;
  s32 i;
  s32 j;
  s32 kind;
  s32 owner;

  found = 0;
  kind = 1;
  if (arg0 > 7)
    kind = 2;
  total = BattleParty_ListActorIds(kind, (u16 *)values);
  owner = 0;
  if (arg0 > 7)
    owner = 1;
  list = &Djinn_GetRecoveryTableFar(owner)->list;
  if (counts != 0)
    for (j = 3; j >= 0; j--)
      counts[j] = 0;
  i = 0;
  if (list->count != 0) {
    do {
      if (list->entries[i].turns == -1) {
        for (j = 0; j < total; j++)
          if (values[j] == list->entries[i].unitId)
            break;
        if (j != total) {
          if (counts != 0)
            counts[list->entries[i].element]++;
          found++;
        }
      }
      i++;
    } while (i != list->count);
  }
  return found;
}

s32 Battle_GetTaggedSlotValue(s32 tag);

s32 Func_0812381c(struct BattleActionRecord *request, struct BattlePlan *plan) {
  struct BattleUnit *actor = Unit_Get__f11f66c(request->unitId);
  struct ActionRunBattleWork *battle =
      (struct ActionRunBattleWork *)Ram_HeapSlots->battleWork;
  s32 selectedTargetId;
  s32 abilityId;

  s32 Func_08123648(s32 abilityId) {
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
    case 0:
      plan->targetCount = 1;
      plan->targetOffset[0] = mode;
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
      targetCount = 0;
      range = request->range;
      for (pass = 0; pass <= 1; pass++) {
        for (place = 0; battle->partyUnits[place] != 255; place++)
          ;
        partyCount = place;
        for (place = 0; battle->enemyIds[place] != 255; place++)
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
              if (targetId == 254)
                continue;
              if (!includeDowned &&
                  ((struct BattleUnit *)Unit_Get__f11f66c(targetId))->hp == 0)
                continue;
              plan->targetMultiplier[targetCount] = 1;
              plan->targetOffset[targetCount] = place - aimedPlace;
              plan->targetIds[targetCount] = targetId;
              targetCount++;
            } else {
              if (place >= partyCount)
                continue;
              targetId = battle->partyUnits[place];
              if (targetId == 254)
                continue;
              if (!includeDowned &&
                  ((struct BattleUnit *)Unit_Get__f11f66c(targetId))->hp == 0)
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
          MessageArgument_PushFar(request->unitId, 1);
          BattleMessage_ShowAndWaitFar((s32)&MsgActorDefends);
          if (actor->guardLevel == 0)
            actor->guardLevel = 1;
          return -1;
        }
        if (ability->effect != 75)
          break;
        range = 11;
      }
      break;
    }
    return 0;
  }

  selectedTargetId = Battle_GetTaggedSlotValue(request->target);
  BattlePlayback_Reset();
  ((s32 *)plan)[25] = 0;
  plan->hauntDamage = 0;
  plan->actorId = request->unitId;
  plan->targetCount = 0;
  plan->presentationFlags = 0;
  plan->failure = 0;
  plan->actorId2 = request->unitId;
  plan->element = 4;
  MessageArgument_ClearAllFar();
  if (actor->hp == 0)
    return -2;

  if (gDebugMode && GameFlag_Test(365) && (gInput.held & 0x100)) {
    s32 enemy = 1;
    s32 index;
    s32 id;

    // The held keys reread through a volatile pointer, a load apart from the R
    // test's; as gInput.held, 12 of 1601 instructions differ in tla-en.
    if (KEYS_HELD & 4)
      enemy = 0;
    for (index = 0;; index++) {
      if (enemy)
        id = battle->enemyIds[index];
      else
        id = battle->partyUnits[index];
      if (id == 255)
        break;
      if (id == 254)
        continue;
      if (Unit_AddHpFar(id, 0xc0000000) == 0) {
        BattleEvent_Push(BATTLE_EVENT_HIT, id);
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, id);
      }
    }
    BattleEvent_DispatchQueued();
    return -2;
  }

  MessageArgument_ClearAllFar();
  if (request->command != 9) {
    if (actor->immobile) {
      actor->immobile = 0;
      MessageArgument_PushFar(request->unitId, 1);
      BattleMessage_ShowAndWaitFar((s32)&MsgUnableToMove);
      return -1;
    }
    if (actor->sleep) {
      MessageArgument_PushFar(request->unitId, 1);
      BattleMessage_ShowAndWaitFar((s32)&MsgIsAsleep);
      return -1;
    }
    if (actor->stun) {
      MessageArgument_PushFar(request->unitId, 1);
      BattleMessage_ShowAndWaitFar((s32)&MsgIsParalyzed);
      return -1;
    }
    if ((actor->restraint & 1) && request->command != COMMAND_DEFEND &&
        (Battle_RandomFar() & 3) == 0) {
      MessageArgument_PushFar(request->unitId, 1);
      BattleMessage_ShowAndWaitFar((s32)&MsgIsBound);
      return -1;
    }
  }
  if (request->command == COMMAND_NONE)
    return -2;
  abilityId = 1;
  {
    s32 index;

    for (index = 0; index < 14; index++)
      plan->targetCritical[index] = 0;
    for (index = 0; index < 14; index++)
      plan->targetHit[index] = -1;
  }

  switch (request->command) {
  case COMMAND_FLEE:
    if ((u16)request->unitId <= 7) {
      BattleMessage_ShowAndWaitFar((s32)&MsgPartyFlees);
    } else {
      MessageArgument_PushFar(request->unitId, 1);
      BattleMessage_ShowAndWaitFar((s32)&MsgActorRuns);
    }
    BattleMessage_WaitForAdvance();
    plan->outcome = 7;
    return 0;

  case COMMAND_ATTACK:
    abilityId = RollWeaponUnleashFar(actor);
    if (Func_08123648(abilityId) == -1)
      return -1;
    if (abilityId != 1) {
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(Inventory_GetEquippedItemFar(actor, 1), 2);
      BattleMessage_ShowAndWaitFar((s32)&MsgWeaponHowls);
      Func_08120158();
      MessageArgument_PushFar(abilityId, 4);
      BattleMessage_ShowAndWaitFar((s32)(MsgWeaponHowls + 1));
    }
    break;

  case 9:
    abilityId = 299;
    if ((u16)(actor->enemyId - 101) <= 2)
      abilityId = 1;
    if (selectedTargetId == 255)
      return -2;
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
    if (Func_08123648(abilityId) == -1)
      return -1;
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    switch (abilityId) {
    case 600 ... 627:
      BattleMessage_ShowAndWaitFar((s32)&MsgActorCastsSpecial);
      break;
    case 590 ... 597:
      BattleMessage_ShowAndWaitFar((s32)&MsgActorCastsDjinni);
      break;
    default:
      BattleMessage_ShowAndWaitFar((s32)&MsgActorCasts);
      break;
    }
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
      Unit_RecalculateGaugesFar(request->unitId);
      if (actor->pp < 0)
        actor->pp = 0;
      if (actor->pp > actor->maxPp)
        actor->pp = actor->maxPp;
    }
  } break;

  case COMMAND_ITEM: {
    struct ItemDefinition *item;

    if (request->parameter < 0) {
      MessageArgument_PushFar(request->unitId, 1);
      BattleMessage_ShowAndWaitFar((s32)&MsgItemAlreadyUsed);
      return -1;
    }
    item = Item_Get(actor->inventory[request->parameter]);
    abilityId = item->actionId;
    if (abilityId == 0 || (actor->inventory[request->parameter] & 0x400)) {
      MessageArgument_PushFar(request->unitId, 1);
      BattleMessage_ShowAndWaitFar((s32)&MsgActorDefends);
      if (actor->guardLevel == 0)
        actor->guardLevel = 1;
      return -1;
    }
    if (Func_08123648(abilityId) == -1)
      return -1;
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(actor->inventory[request->parameter], 2);
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
    MessageArgument_PushFar(request->unitId, 1);
    BattleMessage_ShowAndWaitFar((s32)&MsgActorDefends);
    return -1;

  case COMMAND_NONE:
    return -2;

  case COMMAND_ABILITY: {
    s32 message;

    abilityId = request->parameter;
    if (Func_08123648(abilityId) == -1)
      return -1;
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    if ((Ability_Get(abilityId)->flags & 15) == 6)
      message = (s32)&MsgActorUnleashesAbility;
    else
      message = (s32)&MsgActorUsesAbility;
    switch (abilityId) {
    case 435:
    case 436:
      message = (s32)&MsgEnemyAbility0d53;
      break;
    case 224:
      message = (s32)&MsgActorCasts;
      break;
    case 500:
      message = (s32)&MsgEnemyAbility0d58;
      break;
    case 501:
      message = (s32)&MsgEnemyAbility0d59;
      break;
    case 499:
      message = (s32)&MsgEnemyAbility0d5a;
      break;
    case 494:
      message = (s32)&MsgEnemyAbility0d5b;
      break;
    case 437:
    case 438:
    case 439:
    case 440:
    case 441:
      message = (s32)&MsgEnemyAbility0d5c;
      break;
    case 442:
    case 443:
    case 444:
      message = (s32)&MsgActorUsesAbility;
      break;
    case 472:
      message = (s32)&MsgEnemyAbility0d5d;
      break;
    case 488:
      message = (s32)&MsgEnemyAbility0d5e;
      break;
    case 492:
      message = (s32)&MsgEnemyAbility0d60;
      break;
    case 495:
      message = (s32)&MsgEnemyAbility0d5f;
      break;
    case 503:
      message = (s32)&MsgEnemyAbility0d61;
      break;
    case 504:
      message = (s32)&MsgEnemyAbility0d62;
      break;
    case 508:
      message = (s32)&MsgEnemyAbility0d63;
      break;
    case 733:
      message = (s32)&MsgEnemyAbility0d65;
      break;
    }
    BattleMessage_ShowAndWaitFar(message);
  } break;

  case COMMAND_DJINN:
    abilityId = Djinn_GetDefinitionHeaderFar((request->parameter >> 8) & 15,
                                             request->parameter & 255);
    if (!Djinn_IsSetFar(request->unitId, (request->parameter >> 8) & 15,
                        request->parameter & 255)) {
      if (Djinn_CanSetFar(request->unitId, (request->parameter >> 8) & 15,
                          request->parameter & 255)) {
        Ability_Get(abilityId);
        BattleHighlight_SetModes(0, 0);
        Djinn_SetFar(request->unitId, (request->parameter >> 8) & 15,
                     request->parameter & 255);
        DjinnRecovery_RemoveFar(request->unitId, (request->parameter >> 8) & 15,
                                request->parameter & 255);
        Unit_CalculateFar(request->unitId);
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(30);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, request->unitId);
        BattleEvent_Push(BATTLE_EVENT_ABILITY_NAME,
                         ((request->parameter >> 8) & 15) * 20 +
                             (request->parameter & 255) + 300);
        BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
        BattleEvent_Push(BATTLE_EVENT_DRAW_STATUS, 0);
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)&MsgDjinnIsSet);
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, request->unitId);
        Sound_PlayFar(212);
        Actor_SetMode(BattleObject_Get(request->unitId)->object, 3);
        Actor_SetAnimationSpeedFar(
            (struct Actor *)BattleObject_Get(request->unitId)->object, 32);
        Func_08127308(request->unitId, (request->parameter >> 8) & 15, 3, 0);
        BattlePlayback_WaitForReady();
        return -2;
      }
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      Sound_PlayFar(114);
      BattleMessage_ShowAndWaitFar((s32)&MsgDjinnInRecovery);
      wait(60);
      return -1;
    }
    {
      struct BattleAct *ability;

      if (Func_08123648(abilityId) == -1)
        return -1;
      DjinnRecovery_AddStandbyFar(request->unitId,
                                  (request->parameter >> 8) & 15,
                                  request->parameter & 255);
      ability = Ability_Get(abilityId);
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
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
    BattleParty_CountStandbyDjinn(
        ((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided
            ? request->unitId
            : 0,
        counts);
    side = 0;
    if ((u16)request->unitId > 7 &&
        ((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided)
      side = 1;
    list = (struct DjinnRecoveryListDraft *)((u8 *)Djinn_GetRecoveryTableFar(
                                                 side) +
                                             8);
    for (index = 0; index < 4 && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->nameMessageId;
    if (Func_08123648(abilityId) == -1)
      return -1;
    if (index != 4) {
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      BattleMessage_ShowAndWaitFar((s32)&MsgSummonLacksDjinn);
      return -1;
    }
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    BattleMessage_ShowAndWaitFar((s32)&MsgActorSummons);
    for (index = 0; index != list->count; index++) {
      struct DjinnRecoveryEntryDraft *entry = &list->entries[index];

      if (entry->turns == -1 && BattleParty_IsUnitListed(entry->unitId) &&
          counts[entry->element] != 0) {
        entry->turns = -2;
        counts[entry->element]--;
      }
    }
    if (abilityId == 402) {
      if ((u16)request->unitId > 7)
        battle->summonedMask |= 1 << battle->summonCount;
      battle->summonCount++;
    }
  } break;
  }

  if (abilityId == 1) {
    struct BattleUnit *target = Unit_Get__f11f66c(plan->targetIds[0]);

    plan->abilityId = 1;
    plan->element = Item_GetEquippedElementFar(request->unitId);
    plan->outcome = 2;
    if (actor->classIndex == 0) {
      plan->presentationFlags =
          BattleClass_GetAttackEffect(actor->enemyId) | 0x4000;
    } else {
      plan->presentationFlags = 0;
      plan->presentationFlags = Func_0811d79c(actor->enemyId);
    }
    MessageArgument_PushFar(request->unitId, 1);
    BattleMessage_ShowAndWaitFar((s32)&MsgActorAttacks);
    // The rolls in a one-pass block left by break, which places them ahead of
    // the five tests; as an if on the negated tests, 119 of 1601 instructions
    // differ in tla-en.
    do {
      if (target->hp == 0 || target->sleep != 0 || target->stun != 0 ||
          target->immobile != 0 || target->charm != 0)
        break;
      if (actor->delusion != 0 && (Battle_RandomFar() & 255) <= 152)
        plan->targetMultiplier[0] = 0;
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetMultiplier[0] = 0;
    } while (0);
    if (GameFlag_Test(366))
      plan->targetMultiplier[0] = 0;
    if (target->hp != 0) {
      if ((Battle_RandomFar() & 31) == 0)
        plan->targetCritical[0] = 1;
      else if ((Equipment_GetUnleashRateBonusFar(actor) << 16) / 200 >
               (Battle_RandomFar() & 0xffff))
        plan->targetCritical[0] = 1;
    }
  } else {
    struct BattleAct *ability = Ability_Get(abilityId);
    s32 chance;
    s32 amount;

    plan->abilityId = abilityId;
    plan->element = ability->element;
    plan->presentationFlags = 0;
    if (ability->effect == 65 || ability->effect == 41 ||
        ability->effect == 42 || ability->effect == 43 ||
        ability->effect == 44 || ability->effect == 68 ||
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
        if (ability->effect == 65 || ability->effect == 68)
          chance = 153;
        else if (ability->effect == 41 || ability->effect == 43)
          chance = 32;
        else
          chance = 64;
        if (ability->effect == 65 || ability->effect == 41 ||
            ability->effect == 42)
          amount = 1;
        else
          amount = 2;
        if ((Battle_RandomFar() & 255) < chance) {
          for (index = 0; index < plan->targetCount; index++)
            plan->targetMultiplier[index] += amount;
        }
      }
    } else if (ability->effect >= 36 && ability->effect <= 40) {
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
      if ((Battle_RandomFar() & mask) == 0) {
        s32 index;

        for (index = 0; index < plan->targetCount; index++)
          plan->targetCritical[index] = 2;
      }
    } else if (abilityId == 178 || abilityId == 374 || ability->effect == 82) {
      s32 index;

      for (index = 0; index < plan->targetCount; index++) {
        s32 rate;

        if (ability->targetMode == 1) {
          rate = Data_0812896c[__builtin_abs(plan->targetOffset[index])];
        } else {
          rate = 100;
        }
        plan->targetHit[index] = Battle_RollEffectSuccessFar(
            request->unitId, plan->targetIds[index], ability->element,
            ability->effect, rate);
      }
    }
    if (abilityId == 312) {
      plan->actorId2 = Func_0811f5a4(plan->actorId);
    } else if (abilityId == 316) {
      s32 tries = 0;

      plan->actorId2 = plan->targetIds[0];
      do {
        plan->targetIds[0] =
            Battle_GetTaggedSlotValue(Func_0811f4d4(plan->actorId2 <= 7));
        if (plan->targetIds[0] != plan->actorId2)
          break;
      } while (++tries <= 9);
    }
    if (abilityId >= 0 && abilityId <= 732) {
      plan->presentationFlags = Data_08128c50[abilityId];
      if (plan->targetMultiplier[0] > 1)
        plan->presentationFlags = Data_08128c50[abilityId] +
                                  (plan->targetMultiplier[0] << 12) - 0x1000;
    }
    if ((abilityId >= 0 && abilityId <= 732) && Data_08128972[abilityId] != 0)
      plan->outcome = Data_08128972[abilityId];
    else if (Ability_IsPsynergy(abilityId))
      plan->outcome = 3;
    else if (plan->presentationFlags != 0) {
      if (actor->classIndex == 0)
        plan->outcome = 8;
      else
        plan->outcome = 3;
    } else
      plan->outcome = 1;
    if (Battle_IsReviveEffectFar(ability->effect))
      plan->presentationFlags |= 0x10000;
    if ((abilityId == 178 || abilityId == 374 || ability->effect == 82) &&
        plan->targetHit[0] != 0)
      plan->presentationFlags |= 0x1000;
  }
  if (request->command == COMMAND_ITEM && plan->outcome != 5 &&
      plan->outcome != 9)
    plan->outcome = 4;
  plan->command = request->command;
  return 0;
}
