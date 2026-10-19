/// @file
/// Part of battle.c, the battle object, compiled only through it.
/// An enemy's choice of action, BattleCommand_SelectAutomatic.

#include "../battle_internal.h"

/// An enemy's memory of its action choice, kept in BattleUnit.rewards.
struct BattleAutomaticState {
  unsigned initialized : 1;
  unsigned selection : 3; ///< the place in its action list
  unsigned remaining : 28;
};

/// Chooses an enemy's action (a party member's is chosen in the menu): one of
/// the eight in its definition, picked by its action strategy, with an item
/// instead where its item mask says so, and a target worth the ability. A
/// choice that cannot be made (too little PP, sealed, a forbidden escape, no
/// worthwhile target) picks again; after 18 tries the enemy defends.
/// retry : 0 when the round is built: only a guard is settled now (it must
///         raise before anyone acts); 1 at the enemy's turn: the rest.
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *request,
                                  s32 retry) {
  u32 selection;
  struct BattleAutomaticState *selectionMemory;
  struct EnemyDefinition *enemy;
  s32 itemsAllowed;
  s32 failedChoices;
  s32 needsChoice;
  s8 *actionStrategy;
  u8 *itemActionMask;
  s16 command;
  s32 allyTarget;
  s32 opponentTarget;
  s32 allyIsParty;
  s32 opponentIsParty;
  struct ItemDefinition *item;
  s32 useItem;
  s8 strategyKind;
  struct BattleAct *itemAction;
  struct BattleAct *ability;
  struct BattleUnit *unit;
  s32 abilityId;
  s32 flags;
  u8 effect;
  u8 targetMode;

  // Picks a place in the action list from a 0-255 roll and eight weights.
  s32 BattleEnemy_SelectWeightedAction(u8 *weights) {
    s32 roll;
    s32 total;
    s32 selectedAction;
    s32 actionIndex;

    roll = BattleRandomFar() & 0xff;
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

  unit = UnitGetFar(request->unitId);
  needsChoice = itemsAllowed = 1;
  failedChoices = 0;
  selection = -1U;
  // A party member chooses in the menu.
  if (unit->classIndex != 0)
    return;
  if (retry != 0 && request->command != CMD_ABILITY)
    return;

  enemy = Enemy_GetDefinitionFar(unit->classId);
  selectionMemory = (struct BattleAutomaticState *)&unit->rewards;
  actionStrategy = &enemy->actionStrategy;
  itemActionMask = &enemy->itemActions;

  // A place in the action list, by the enemy's strategy.
select_again:
  strategyKind = *actionStrategy;
  switch (strategyKind) {
  case 0: // the first weight table
    selection =
        BattleEnemy_SelectWeightedAction(gBattleCommand_SelectionWeights[0]);
    break;
  case 1:
    selection =
        BattleEnemy_SelectWeightedAction(gBattleCommand_SelectionWeights[1]);
    break;
  case 2:
    selection =
        BattleEnemy_SelectWeightedAction(gBattleCommand_SelectionWeights[2]);
    break;
  case 3: // from a random place, then in order
    if (selectionMemory->initialized == 0) {
      selectionMemory->selection = BattleRandomFar();
      selectionMemory->initialized = 1;
    }
    // fall through
  case 4: // in order from the remembered place
    selection = selectionMemory->selection;
    if (retry != 0) {
      selectionMemory->selection = selection + 1;
    }
    break;
  case 5: // the next place at each new pick
    selection++;
    break;
  case 6: // the same place
    break;
  }

  // A place marked for an item: use the first item held, if any.
  useItem = (*itemActionMask >> selection) & 1;
  useItem &= itemsAllowed;
  abilityId = enemy->actions[selection];
  request->command = CMD_ABILITY;
  if (useItem != 0 && retry != 0) {
    if (!(ITEM_ID_MASK & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == 0) {
        request->command = CMD_ITEM;
        request->parameter = 0x1fd;
        return;
      }
    }
    if (useItem != 0) {
      item = Item_GetDirectFar(unit->inventory[0]);
      if (item->useType == ITEM_USE_CONSUMED) {
        itemAction = BattleActGet(item->actionId);
        abilityId = item->actionId;
        request->command = CMD_ITEM;
        flags = itemAction->flags;
        request->parameter = 0;
        switch (flags) {
        case 1:
        case 2:
          break;
        default:
          useItem = 0;
          break;
        }
      } else {
        useItem = 0;
      }
    }
    if (useItem == 0)
      itemsAllowed = 0;
  }

  if (needsChoice != 0) {
    // A guard or an escape targets the user itself.
    ability = BattleActGet(abilityId);
    effect = ability->effect;
    switch (effect) {
    case ABILITY_EFFECT_DEFEND:
      request->command = CMD_DEFEND;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_TOTAL_DEFENSE:
      request->command = CMD_DEFEND2;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_FLEE:
      request->command = CMD_FLEE;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    }

    if (retry == 0) {
      switch (request->command) {
      case CMD_DEFEND:
      case CMD_DEFEND2:
        break;
      default:
        goto done;
      }
    }

    // Psynergy: a careful enemy (strategy not 0) avoids it without the PP,
    // and the most careful (2) also while sealed.
    if (request->command != CMD_ITEM && Ability_IsPsynergy(abilityId) != 0) {
      request->command = CMD_PSYNERGY;
      request->parameter = abilityId;
      if (ability->ppCost > unit->pp && enemy->targetStrategy != 0)
        goto next_candidate;
      if (unit->psySeal != 0 && enemy->targetStrategy == 2)
        goto next_candidate;
    }
    command = request->command;
    if (command == CMD_FLEE && (u8)unit->challenge != 0)
      goto next_candidate;
    if (retry != 0 && (command == CMD_DEFEND || command == CMD_DEFEND2))
      goto next_candidate;

    if (command == CMD_ABILITY) {
      request->parameter = abilityId;
      if (abilityId == ABILITY_ATTACK)
        request->command = CMD_ATTACK;
    }

    // The target.
    request->range = ability->range;
    targetMode = ability->targetMode;
    switch (targetMode) {
    case TARGET_MODE_ALLIES:
    case TARGET_MODE_SELF:
      allyTarget = BattleTarget_SelectForAction(request->unitId, ability);
      if (allyTarget == -2) {
        allyIsParty = 0;
        if (request->unitId <= (u32)ALLY_MAX)
          allyIsParty = 1;
        allyTarget = BattleTarget_SelectRandomPosition(allyIsParty);
      }
      if (allyTarget != -1) {
        request->target = allyTarget;
        needsChoice = 0;
      }
      break;
    case TARGET_MODE_FOES:
      opponentTarget = BattleTarget_SelectForAction(request->unitId, ability);
      if (opponentTarget == -2) {
        opponentIsParty = 0;
        if (request->unitId <= (u32)ALLY_MAX)
          opponentIsParty = 1;
        opponentTarget = BattleTarget_SelectRandomPosition(opponentIsParty);
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
      opponentTarget = Battle_FindTaggedSlotByValue(request->unitId);
      request->target = opponentTarget;
      needsChoice = 0;
      break;
    }
  }
  if (retry == 0)
    needsChoice = 0;
next_candidate:
  // After 18 failed picks, defend.
  if (needsChoice != 0 && failedChoices > 16) {
    request->command = CMD_DEFEND;
    needsChoice = 0;
  }
  failedChoices++;
  if (needsChoice != 0)
    goto select_again;
done:
  return;
}
