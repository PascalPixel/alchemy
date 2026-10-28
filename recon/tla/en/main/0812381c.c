// Draft of Func_0812381c with its nested Func_08123648 (compare both:
// alchemy-devtools diff 0812381c.c 08123648,0812381c): ☀️'s BattleActionRun_Build
// and BattleActionRun_SetTargets (games/tbs/src/battle/battle/action.c), ported.
// ⚓️ differs: the plan's second unit (its byte 2) and its word at 100 are
// cleared with it; command 9 skips the checks for what stops the actor and
// attacks the aimed unit (ability 299, or the plain attack for enemies 101 to
// 103); Psynergy 590 to 597 and 600 to 627 take their own cast messages; a
// summon counts the standby Djinn of both sides in a link battle, and summon
// 402 records which side summoned it in the battle work (0x862, 0x865);
// effect 89 doubles or triples a blow 179 times in 256; abilities 178 and 374
// and effect 82 roll their hit by the distance table Data_0812896c (the plain
// rate 100 unless they aim one unit); ability 312 picks a second unit with
// Func_0811f5a4 and 316 re-aims at random up to ten times; the attack's
// animation comes from Func_0811d79c for a party member. The nested function
// counts targets twice for effect 75, the second time with range 11.
// Message names are ours where en.po has none: the enemy abilities' are
// numbered (MsgEnemyAbility0d53 and on), the rest ☀️'s.
// Near miss, differ 84 of 1827 (2026-10-24), instruction order only: 20 in the
// nested function, the rest in the parent, each a load the native issues one
// slot earlier, a store the native lets one instruction pass before the next
// load, or a constant's register (the plan's first stores, command 9's).
// Forms measured: the enemy ability messages switched in ☀️'s order (the case
// bodies' layout follows it), the Psynergy messages as a switch on ranges
// (if-else chains fold into a range test or split the default's call), the
// attack's distance through abs() (the native's compare and negate around a
// byte load), the held keys through a volatile view, the animation table
// const (its value is reused after the store), one amount and chance for the
// two blow cases and one index for their loops.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_event.h"
#include "battle_party.h"
#include "battle_summon.h"
#include "battle_system.h"
#include "game/item.h"
#include "motobj.h"
#include "input.h"
#include "ram_buffer.h"

struct ActionRunBattleWork {
  u8 unk000[0x44];
  u8 twoSided;
  u8 unk045[0x58 - 0x45];
  s16 partyUnits[7];
  s16 enemyIds[7];
  u8 unk074[0x862 - 0x74];
  u16 summonedMask;
  u8 unk864;
  u8 summonCount;
};

struct DjinnRecoveryEntryDraft {
  u8 element;
  u8 index;
  u8 unitId;
  s8 turns;
};

struct DjinnRecoveryListDraft {
  struct DjinnRecoveryEntryDraft entries[72];
  s32 count;
};

#define KEYS_HELD (*(volatile u32 *)&gInput.held)

extern u8 gDebugMode;
extern const u32 Data_08128c50[];
extern u8 Data_08128972[];
extern u8 Data_0812896c[];

extern char MsgActorDefends;
extern char MsgUnableToMove;
extern char MsgIsAsleep;
extern char MsgIsParalyzed;
extern char MsgIsBound;
extern char MsgPartyFlees;
extern char MsgActorRuns;
extern char MsgWeaponHowls;
extern char MsgActorCastsSpecial;
extern char MsgActorCastsDjinni;
extern char MsgActorCasts;
extern char MsgItemAlreadyUsed;
extern char MsgActorRaisesItem;
extern char MsgActorUsesBattleItem;
extern char MsgActorUnleashesAbility;
extern char MsgActorUsesAbility;
extern char MsgEnemyAbility0d53;
extern char MsgEnemyAbility0d58;
extern char MsgEnemyAbility0d59;
extern char MsgEnemyAbility0d5a;
extern char MsgEnemyAbility0d5b;
extern char MsgEnemyAbility0d5c;
extern char MsgEnemyAbility0d5d;
extern char MsgEnemyAbility0d5e;
extern char MsgEnemyAbility0d5f;
extern char MsgEnemyAbility0d60;
extern char MsgEnemyAbility0d61;
extern char MsgEnemyAbility0d62;
extern char MsgEnemyAbility0d63;
extern char MsgEnemyAbility0d65;
extern char MsgDjinnSet;
extern char MsgDjinnInRecovery;
extern char MsgActorUnleashesDjinn;
extern char MsgSummonLacksDjinn;
extern char MsgActorSummons;
extern char MsgActorAttacks;

struct BattleUnit *Unit_Get(s32 unitId);
s32 Battle_GetTaggedSlotValue(s32 tag);
void BattleEventRuntime_Reset(void);
void BattleEventRuntime_SchedulePhase(s32 phase);
void BattleEventRuntime_WaitForReady(void);
s32 Unit_AddHpFar(s32 owner, s32 delta);
s32 GameFlag_Test(s32 flag);
s32 RollWeaponUnleashFar(struct BattleUnit *owner);
s32 Inventory_GetEquippedItemFar(struct BattleUnit *owner, s32 slot);
void Func_08120158(void);
s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 unitId, s32 element, s32 index);
s32 Djinn_IsReadyFar(s32 unitId, s32 element, s32 index);
s32 Djinn_ActivateFar(s32 unitId, s32 element, s32 index);
s32 DjinnRecovery_RemoveFar(s32 unitId, s32 element, s32 index);
void DjinnRecovery_AddStandbyFar(s32 unitId, s32 element, s32 index);
void Unit_CalculateFar(s32);
void Sound_PlayFar(s32 cue);
void Actor_SetMode(struct MotionObject *object, s32 mode);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void Func_08123574(s32 side, u8 *counts);
u32 BattleParty_IsUnitListed(u32 id);
s32 Item_GetEquippedElementFar(s32 unitId);
s32 BattleClass_GetAttackEffect(s32 classId);
s32 Func_0811d79c(s32 classId);
s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *owner);
s32 BattleEffect_RollSuccessFar(s32 actor, s32 target, s32 element, s32 effect,
                                s32 rate);
s32 Func_0811f5a4(s32 unitId);
s32 Func_0811f4d4(s32 party);

s32 Func_0812381c(struct BattleActionRecord *request, struct BattlePlan *plan) {
  struct BattleUnit *actor = Unit_Get(request->unitId);
  struct ActionRunBattleWork *battle =
      (struct ActionRunBattleWork *)Ram_HeapSlots->battleWork;
  s32 selectedTargetId;
  s32 abilityId;

  s32 Func_08123648(s32 abilityId) {
    struct BattleAct *ability = BattleAction_Get(abilityId);
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
              if (targetId == 254)
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
          MessageArgument_PushFar(request->unitId, 1);
          Message_ShowAndWaitCoreFar((s32)&MsgActorDefends);
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
  BattleEventRuntime_Reset();
  ((s32 *)plan)[25] = 0;
  plan->hauntDamage = 0;
  plan->targetCount = 0;
  plan->presentationFlags = 0;
  plan->failure = 0;
  plan->actorId = request->unitId;
  plan->actorId2 = request->unitId;
  plan->element = 4;
  MessageArgument_ClearAllFar();
  if (actor->hp == 0)
    return -2;

  if (gDebugMode && GameFlag_Test(365) && (KEYS_HELD & 0x100)) {
    s32 enemy = 1;
    s32 index;
    s32 id;

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
      Message_ShowAndWaitCoreFar((s32)&MsgUnableToMove);
      return -1;
    }
    if (actor->sleep) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgIsAsleep);
      return -1;
    }
    if (actor->stun) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgIsParalyzed);
      return -1;
    }
    if ((actor->restraint & 1) && request->command != COMMAND_DEFEND &&
        (Battle_RandomFar() & 3) == 0) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgIsBound);
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
      Message_ShowAndWaitCoreFar((s32)&MsgPartyFlees);
    } else {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgActorRuns);
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
      Message_ShowAndWaitCoreFar((s32)&MsgWeaponHowls);
      Func_08120158();
      MessageArgument_PushFar(abilityId, 4);
      Message_ShowAndWaitCoreFar((s32)(&MsgWeaponHowls + 1));
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
    plan->targetMultiplier[0] = 1;
    plan->targetIds[0] = selectedTargetId;
    break;

  case COMMAND_PSYNERGY: {
    struct BattleAct *ability;
    s32 canCast = 1;

    abilityId = request->parameter;
    ability = BattleAction_Get(abilityId);
    if (Func_08123648(abilityId) == -1)
      return -1;
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    switch (abilityId) {
    case 600 ... 627:
      Message_ShowAndWaitCoreFar((s32)&MsgActorCastsSpecial);
      break;
    case 590 ... 597:
      Message_ShowAndWaitCoreFar((s32)&MsgActorCastsDjinni);
      break;
    default:
      Message_ShowAndWaitCoreFar((s32)&MsgActorCasts);
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
      Unit_UpdateRatiosFar(request->unitId);
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
      Message_ShowAndWaitCoreFar((s32)&MsgItemAlreadyUsed);
      return -1;
    }
    item = Item_Get(actor->inventory[request->parameter]);
    abilityId = item->actionId;
    if (abilityId == 0 || (actor->inventory[request->parameter] & 0x400)) {
      MessageArgument_PushFar(request->unitId, 1);
      Message_ShowAndWaitCoreFar((s32)&MsgActorDefends);
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
        Message_ShowAndWaitCoreFar((s32)&MsgActorRaisesItem);
        break;
      default:
        Message_ShowAndWaitCoreFar((s32)&MsgActorUsesBattleItem);
        break;
      }
    } else {
      Message_ShowAndWaitCoreFar((s32)&MsgActorUsesBattleItem);
    }
  } break;

  case COMMAND_DEFEND:
  case COMMAND_DEFEND2:
    MessageArgument_PushFar(request->unitId, 1);
    Message_ShowAndWaitCoreFar((s32)&MsgActorDefends);
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
    if ((BattleAction_Get(abilityId)->flags & 15) == 6)
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
    Message_ShowAndWaitCoreFar(message);
  } break;

  case COMMAND_DJINN:
    abilityId = Djinn_GetDefinitionHeaderFar((request->parameter >> 8) & 15,
                                             request->parameter & 255);
    if (!Djinn_IsActiveFar(request->unitId, (request->parameter >> 8) & 15,
                           request->parameter & 255)) {
      if (Djinn_IsReadyFar(request->unitId, (request->parameter >> 8) & 15,
                           request->parameter & 255)) {
        BattleAction_Get(abilityId);
        BattleHighlight_SetModes(0, 0);
        Djinn_ActivateFar(request->unitId, (request->parameter >> 8) & 15,
                          request->parameter & 255);
        DjinnRecovery_RemoveFar(request->unitId, (request->parameter >> 8) & 15,
                                request->parameter & 255);
        Unit_CalculateFar(request->unitId);
        BattleEventRuntime_Reset();
        BattleEventRuntime_SchedulePhase(30);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, request->unitId);
        BattleEvent_Push(BATTLE_EVENT_ABILITY_NAME,
                         ((request->parameter >> 8) & 15) * 20 +
                             (request->parameter & 255) + 300);
        BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
        BattleEvent_Push(BATTLE_EVENT_DRAW_STATUS, 0);
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)&MsgDjinnSet);
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, request->unitId);
        Sound_PlayFar(212);
        Actor_SetMode(BattleObject_Get(request->unitId)->object, 3);
        Actor_SetAnimationSpeedFar(
            (struct DispatchObject *)BattleObject_Get(request->unitId)->object,
            32);
        Func_08127308(request->unitId, (request->parameter >> 8) & 15, 3, 0);
        BattleEventRuntime_WaitForReady();
        return -2;
      }
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      Sound_PlayFar(114);
      Message_ShowAndWaitCoreFar((s32)&MsgDjinnInRecovery);
      WaitFrames(60);
      return -1;
    }
    {
      struct BattleAct *ability;

      if (Func_08123648(abilityId) == -1)
        return -1;
      DjinnRecovery_AddStandbyFar(request->unitId,
                                  (request->parameter >> 8) & 15,
                                  request->parameter & 255);
      ability = BattleAction_Get(abilityId);
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      Message_ShowAndWaitCoreFar((s32)&MsgActorUnleashesDjinn);
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
    Func_08123574(((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided
                      ? request->unitId
                      : 0,
                  counts);
    side = 0;
    if ((u16)request->unitId > 7 &&
        ((struct ActionRunBattleWork *)Ram_HeapSlots->battleWork)->twoSided)
      side = 1;
    list =
        (struct DjinnRecoveryListDraft *)((u8 *)Djinn_GetRecoveryTableFar(side) + 8);
    for (index = 0; index < 4 && counts[index] >= summon->djinnRequired[index];
         index++)
      counts[index] = summon->djinnRequired[index];
    abilityId = summon->nameMessageId;
    if (Func_08123648(abilityId) == -1)
      return -1;
    if (index != 4) {
      MessageArgument_PushFar(request->unitId, 1);
      MessageArgument_PushFar(abilityId, 4);
      Message_ShowAndWaitCoreFar((s32)&MsgSummonLacksDjinn);
      return -1;
    }
    MessageArgument_PushFar(request->unitId, 1);
    MessageArgument_PushFar(abilityId, 4);
    Message_ShowAndWaitCoreFar((s32)&MsgActorSummons);
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
    struct BattleUnit *target = Unit_Get(plan->targetIds[0]);

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
    Message_ShowAndWaitCoreFar((s32)&MsgActorAttacks);
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
    struct BattleAct *ability = BattleAction_Get(abilityId);
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
        plan->targetHit[index] = BattleEffect_RollSuccessFar(
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
