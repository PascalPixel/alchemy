// Draft of Func_08122514 with its nested Func_081224d8 (compare both:
// alchemy-devtools diff 08122514.c 081224d8,08122514): an enemy's choice of
// action, ☀️'s BattleAI_ChooseAction (games/tbs/src/battle/battle/
// enemy_ai.c) with ⚓️'s additions: enemies 101, 369, 373 and 376 keep their
// action memory per action of the turn in the battle work and take another
// definition for their second to fourth action (and set the battle work's byte
// at 0x869); enemy 218 rotates through the definitions of Data_08128960; an
// in-order enemy's memory still advances when its action is not an ability;
// effects 74 and 88 raise the action's priority by 10000 as the round is
// built; ability 729 summons the last learned summon whose Djinn are on
// standby; abilities 723 and 731 aim with actor 128; effects 86 and 87 need
// room for another enemy; it returns -1 for a party member and 0 otherwise.
// Near miss, differ 42 of 783 (2026-10-24), instruction order only, 14 pairs:
// the native issues a load one or two slots earlier than alchemy-gcc (ahead of
// an independent constant or shift), as ⚓️'s other near misses do, in the
// nested function's roll mask, the opening stores, the two bitfield updates of
// enemy 218's form, the in-order advance, the item-path store, the 10000, the
// summon count call and the closing form update; and the first Djinn
// comparison of the summon search reads required[element] with element in a
// register where the native has folded it to 0 (the native's movs r4, #0 comes
// after both loads; the pointer form *required moves the increment ahead of
// the bound test). Forms measured: unit declared first (its stack slot after
// retry's), selectionMemory before the definition call, return -1 (unit then
// spills and selection takes r11; 230 without), the battle work read through
// the expression at each use (42; 230 with a local per block), the goto loop
// (177; 230 as do-while), and 509 through an s32 temporary (a halfword
// constant otherwise loads from the pool as two halves).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "battle_summon.h"
#include "battle_system.h"
#include "game/item.h"
#include "ram_buffer.h"

struct AIState {
  unsigned initialized : 1;
  unsigned selection : 3;
  unsigned form : 2;
  unsigned remaining : 26;
};

struct AIEnemyDefinition {
  u8 unk00[0x2b];
  s8 targetStrategy;
  s8 actionStrategy;
  u8 itemActions;
  u16 abilities[8];
};

struct AIEnemyRest {
  u8 unk00[0x2c];
  u16 abilities[8];
};

struct AIBattleWork {
  u8 unk000[0x564];
  u16 queuedSummons[2];
  u8 queuedCharges[2];
  s8 queueHead;
  s8 queueCount;
  u8 unk56c[0x869 - 0x56c];
  u8 unk869;
  u8 unk86a[2];
  u32 aiStates[4];
};

#define AI_BATTLE ((struct AIBattleWork *)Ram_HeapSlots->battleWork)

struct BattleUnit *Unit_Get(s32 unitId);
struct AIEnemyDefinition *Enemy_GetDefinitionFar(s32 id);
u32 Random16(void);
s32 Func_0811f66c(s32 actorId, struct BattleAct *ability);
s32 Func_0811f4d4(s32 party);
s32 Battle_FindTaggedSlotByValue(u32 id);
void Func_08123574(s32 side, u8 *counts);
s32 Summon_ListLearnedFar(u8 *list);

extern u8 Data_0812cc5c[];
extern u8 Data_0812cc64[];
extern u8 Data_0812cc6c[];
extern u16 Data_08128960[];

s32 Func_08122514(struct BattleActionRecord *request, s32 retry) {
  struct BattleUnit *unit;
  u32 selection;
  struct AIState *selectionMemory;
  struct AIEnemyDefinition *enemy;
  s32 itemsAllowed;
  s32 failedChoices;
  s32 needsChoice;
  struct AIEnemyRest *halfwords;
  s8 *actionStrategy;
  u8 *itemActionMask;
  s16 command;
  s32 allyTarget;
  s32 opponentTarget;
  s32 allyIsParty;
  s32 opponentIsParty;
  struct AIBattleWork *battle;
  struct ItemDefinition *item;
  s32 useItem;
  s8 strategyKind;
  struct BattleAct *ability;
  s32 abilityId;
  u8 counts[4];
  u8 summons[36];
  s32 summonIndex;
  s32 element;
  const u8 *required;

  s32 Func_081224d8(u8 *weights) {
    s32 roll;
    s32 total;
    s32 selectedAction;
    s32 actionIndex;

    roll = Battle_RandomFar() & 0xff;
    total = weights[0];
    selectedAction = 0;
    actionIndex = 0;
    if (roll >= total) {
      while (++actionIndex <= 7) {
        total += weights[actionIndex];
        if (roll < total) {
          selectedAction = actionIndex;
          break;
        }
      }
    }
    return selectedAction;
  }

  unit = Unit_Get(request->unitId);
  needsChoice = itemsAllowed = 1;
  failedChoices = 0;
  selection = -1U;
  if (unit->classIndex != 0)
    return -1;

  selectionMemory = (struct AIState *)&unit->rewards;
  enemy = Enemy_GetDefinitionFar(unit->enemyId);
  if (unit->enemyId == 101) {
    selectionMemory =
        (struct AIState *)&((struct AIBattleWork *)Ram_HeapSlots->battleWork)
            ->aiStates[request->unk0e];
    if (request->unk0e == 1)
      enemy = Enemy_GetDefinitionFar(102);
  } else if (unit->enemyId == 369) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    switch (request->unk0e) {
    case 1:
      AI_BATTLE->unk869 = 6;
      enemy = Enemy_GetDefinitionFar(371);
      break;
    case 2:
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(370);
      break;
    case 3:
      AI_BATTLE->unk869 = 7;
      enemy = Enemy_GetDefinitionFar(372);
      break;
    }
  } else if (unit->enemyId == 373) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    switch (request->unk0e) {
    case 1:
      AI_BATTLE->unk869 = 6;
      enemy = Enemy_GetDefinitionFar(375);
      break;
    case 2:
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(374);
      break;
    }
  } else if (unit->enemyId == 376) {
    selectionMemory = (struct AIState *)&AI_BATTLE->aiStates[request->unk0e];
    if (request->unk0e == 1) {
      AI_BATTLE->unk869 = 3;
      enemy = Enemy_GetDefinitionFar(377);
    }
  } else if (unit->enemyId == 218) {
    if (selectionMemory->initialized == 0)
      selectionMemory->form = (Random16() * 3) >> 16;
    enemy = Enemy_GetDefinitionFar(Data_08128960[selectionMemory->form]);
  }

  if (retry != 0 && request->command != COMMAND_ABILITY) {
    switch (enemy->actionStrategy) {
    case 3:
    case 4:
      selectionMemory->selection = selectionMemory->selection + 1;
      break;
    }
    goto done;
  }

  actionStrategy = &enemy->actionStrategy;
  itemActionMask = &enemy->itemActions;
  halfwords = (struct AIEnemyRest *)((u8 *)enemy + 2);

select_again:
  strategyKind = *actionStrategy;
  switch (strategyKind) {
  case 0:
    selection = Func_081224d8(Data_0812cc5c);
    break;
  case 1:
    selection = Func_081224d8(Data_0812cc64);
    break;
  case 2:
    selection = Func_081224d8(Data_0812cc6c);
    break;
  case 3:
    if (selectionMemory->initialized == 0) {
      selectionMemory->selection = Battle_RandomFar();
      selectionMemory->initialized = 1;
    }
  case 4:
    selection = selectionMemory->selection;
    if (retry != 0)
      selectionMemory->selection = selection + 1;
    break;
  case 5:
    selection++;
    break;
  case 6:
    break;
  }

  useItem = (*itemActionMask >> selection) & 1;
  useItem &= itemsAllowed;
  abilityId = halfwords->abilities[selection];
  request->command = COMMAND_ABILITY;
  if (useItem != 0 && retry != 0) {
    if (!(0x1ff & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == 0) {
        request->command = COMMAND_ITEM;
        {
          s32 cantUse = 509;

          request->parameter = cantUse;
        }
        goto done;
      }
    }
    if (useItem != 0) {
      item = Item_Get(unit->inventory[0]);
      if (item->useType != 1) {
        useItem = 0;
      } else {
        BattleAction_Get(item->actionId);
        abilityId = item->actionId;
        request->command = COMMAND_ITEM;
        request->parameter = 0;
      }
    }
    if (useItem == 0)
      itemsAllowed = 0;
  }

  if (needsChoice != 0) {
    ability = BattleAction_Get(abilityId);
    switch (ability->effect) {
    case ABILITY_EFFECT_DEFEND:
      request->command = COMMAND_DEFEND;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_TOTAL_DEFENSE:
      request->command = COMMAND_DEFEND2;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_FLEE:
      request->command = COMMAND_FLEE;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case 74:
    case 88:
      if (retry == 0)
        request->priority += 10000;
      break;
    }

    if (retry == 0) {
      switch (request->command) {
      case COMMAND_DEFEND:
      case COMMAND_DEFEND2:
        break;
      default:
        goto done;
      }
    }

    if (request->command != COMMAND_ITEM &&
        Ability_IsPsynergy(abilityId) != 0) {
      request->command = COMMAND_PSYNERGY;
      request->parameter = abilityId;
      if (ability->ppCost > unit->pp && enemy->targetStrategy != 0)
        goto next_candidate;
      if (unit->psySeal != 0 && enemy->targetStrategy == 2)
        goto next_candidate;
    }
    command = request->command;
    if (command == COMMAND_FLEE && (u8)unit->challenge != 0)
      goto next_candidate;
    if (retry != 0 && (command == COMMAND_DEFEND || command == COMMAND_DEFEND2))
      goto next_candidate;

    if (command == COMMAND_ABILITY) {
      request->parameter = abilityId;
      if (abilityId == 1)
        request->command = COMMAND_ATTACK;
      if (abilityId == 729) {
        request->command = COMMAND_SUMMON;
        Func_08123574(0, counts);
        summonIndex = Summon_ListLearnedFar(summons);
        for (summonIndex--; summonIndex >= 0; summonIndex--) {
          required = SummonDefinition_Get(summons[summonIndex])->djinnRequired;
          for (element = 0;
               element <= 3 && required[element] <= counts[element]; element++)
            ;
          if (element == 4)
            break;
        }
        if (summonIndex < 0)
          goto next_candidate;
        request->parameter = summons[summonIndex];
        abilityId = SummonDefinition_Get(summons[summonIndex])->nameMessageId;
        ability = BattleAction_Get(abilityId);
      }
    }

    request->range = ability->range;
    switch (ability->targetMode) {
    case 2:
    case 4:
      if (abilityId == 723 || abilityId == 731)
        allyTarget = Func_0811f66c(128, ability);
      else
        allyTarget = Func_0811f66c(request->unitId, ability);
      if (allyTarget == -2) {
        allyIsParty = 0;
        if ((u16)request->unitId <= 7)
          allyIsParty = 1;
        allyTarget = Func_0811f4d4(allyIsParty);
      }
      if (allyTarget != -1) {
        request->target = allyTarget;
        needsChoice = 0;
      }
      break;
    case 1:
      opponentTarget = Func_0811f66c(request->unitId, ability);
      if (opponentTarget == -2) {
        opponentIsParty = 0;
        if ((u16)request->unitId <= 7)
          opponentIsParty = 1;
        opponentTarget = Func_0811f4d4(opponentIsParty);
      }
      if (opponentTarget != -1) {
        request->target = opponentTarget;
        needsChoice = 0;
      }
      break;
    case 3:
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    default:
      if (ability->effect == 86 &&
          !Enemy_CheckRoom(unit->enemyId == 164 ? 378 : 81))
        break;
      battle = (struct AIBattleWork *)Ram_HeapSlots->battleWork;
      if (ability->effect == 87) {
        if (battle->queueCount == 0)
          break;
        if (!Enemy_CheckRoom(battle->queuedSummons[battle->queueHead]))
          break;
      }
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      needsChoice = 0;
      break;
    }
  }
  if (retry == 0)
    needsChoice = 0;
next_candidate:
  if (needsChoice != 0 && failedChoices > 16) {
    request->command = COMMAND_DEFEND;
    needsChoice = 0;
  }
  failedChoices++;
  if (needsChoice != 0)
    goto select_again;
done:
  if (unit->enemyId == 218 && retry == 1 && selectionMemory->selection == 0)
    selectionMemory->form = (selectionMemory->form + 1) % 3;
  return 0;
}
