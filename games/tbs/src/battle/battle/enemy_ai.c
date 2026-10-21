/// @file
/// An enemy's choice of action, BattleCommand_SelectAutomatic.
// Part of battle.c.

#include "../battle_internal.h"

/// An enemy's memory of its action choice, kept in BattleUnit.rewards (0 in
/// every row of enemies.tsv, so it starts cleared). Only
/// ACTION_STRATEGY_RANDOM_START and ACTION_STRATEGY_IN_ORDER use it.
/// bits: 31..4 unused | 3..1 selection | 0 initialized
struct BattleAutomaticState {
  unsigned initialized
      : 1;                 ///< ACTION_STRATEGY_RANDOM_START has drawn its start
  unsigned selection : 3;  ///< the place in its action list, 0 to 7
  unsigned remaining : 28; ///< not used here
};

/// Chooses an enemy's action (a party member's is chosen in the menu): one of
/// the eight in its definition, picked by its action strategy, with an item
/// instead where its item mask says so, and a target worth the ability. A
/// choice that cannot be made (too little PP, sealed, a forbidden escape, no
/// worthwhile target) picks again; after 18 tries the enemy defends.
/// Called with retry 0 for each enemy as the round is built
/// (BattlePres_BuildOpponentEntries), and with retry 1 at its turn
/// (BattlePresentation_DispatchAction). Writes request's command, parameter,
/// target and range; ACTION_STRATEGY_RANDOM_START and
/// ACTION_STRATEGY_IN_ORDER advance the place remembered in
/// BattleUnit.rewards. Returns nothing.
/// @param retry 0 when the round is built: only a guard is settled now (it
/// must raise before anyone acts); 1 at the enemy's turn: the rest
#ifdef UBFIX
void BattleCommand_SelectAutomatic(struct BattleActionRecord *request,
                                   s32 retry) {
#else
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *request,
                                  s32 retry) {
#endif
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

  /// Picks a place in the action list from a 0-255 roll and eight weights:
  /// the first place whose running total passes the roll, else place 0.
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
  // ACTION_STRATEGY_NEXT_PICK's first pick is place 0, and
  // ACTION_STRATEGY_SAME would keep this -1 as its place; no row of
  // enemies.tsv uses either.
  selection = -1U;
  // A party member chooses in the menu.
  if (unit->classIndex != CLASS_INDEX_ENEMY)
    return;
  if (retry != 0 && request->command != CMD_ABILITY)
    return;

  enemy = (struct EnemyDefinition *)Enemy_GetDefinitionFar(unit->classId);
  selectionMemory = (struct BattleAutomaticState *)&unit->rewards;
  actionStrategy = &enemy->actionStrategy;
  itemActionMask = &enemy->itemActions;

  // A place in the action list, by the enemy's strategy.
select_again:
  strategyKind = *actionStrategy;
  switch (strategyKind) {
  // The three weight tables are in battle_data.s.
  case ACTION_STRATEGY_EVEN:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleCommand_SelectionWeights[ACTION_STRATEGY_EVEN]);
    break;
  case ACTION_STRATEGY_FALLING:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleCommand_SelectionWeights[ACTION_STRATEGY_FALLING]);
    break;
  case ACTION_STRATEGY_FIRST_FAVOURED:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleCommand_SelectionWeights[ACTION_STRATEGY_FIRST_FAVOURED]);
    break;
  case ACTION_STRATEGY_RANDOM_START:
    if (selectionMemory->initialized == 0) {
      selectionMemory->selection = BattleRandomFar();
      selectionMemory->initialized = 1;
    }
    // fall through
  case ACTION_STRATEGY_IN_ORDER:
    // It moves on only at the enemy's turn, wrapping from 7 to 0 in its three
    // bits.
    selection = selectionMemory->selection;
    if (retry != 0) {
      selectionMemory->selection = selection + 1;
    }
    break;
  case ACTION_STRATEGY_NEXT_PICK:
    selection++;
    break;
  case ACTION_STRATEGY_SAME:
    break;
  }

  // A place marked for an item: use the first item held, if any. Once an
  // item is refused, later picks take the place's ability instead.
  useItem = (*itemActionMask >> selection) & 1;
  useItem &= itemsAllowed;
  abilityId = enemy->actions[selection];
  request->command = CMD_ABILITY;
  if (useItem != 0 && retry != 0) {
    if (!(ITEM_ID_MASK & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == STRATEGY_MOST_HP) {
        request->command = CMD_ITEM;
        // XXX: the parameter is the ability Can't Use, yet CMD_ITEM
        // reads its parameter as an inventory slot (BattleCommand_BuildPlan);
        // a Brigand or Rat Fighter (STRATEGY_MOST_HP, item places) out of
        // items would show what the player sees.
        request->parameter = ABILITY_CANT_USE;
        return;
      }
    }
    if (useItem != 0) {
      item = Item_GetDirectFar(unit->inventory[0]);
      // Only an item that is used up, whose ability's flags are exactly
      // ABILITY_KIND_HEAL_HP or ABILITY_KIND_EFFECT, with no higher bit set.
      if (item->useType == ITEM_USE_CONSUMED) {
        itemAction = BattleActGet(item->actionId);
        abilityId = item->actionId;
        request->command = CMD_ITEM;
        flags = itemAction->flags;
        request->parameter = 0;
        switch (flags) {
        case ABILITY_KIND_HEAL_HP:
        case ABILITY_KIND_EFFECT:
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
    // A guard or an escape becomes its own command, aimed at the user itself.
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

    // As the round is built, only a guard is kept; anything else waits for
    // the enemy's turn.
    if (retry == 0) {
      switch (request->command) {
      case CMD_DEFEND:
      case CMD_DEFEND2:
        break;
      default:
        goto done;
      }
    }

    // Psynergy: an enemy whose strategy is not STRATEGY_MOST_HP avoids it
    // without the PP, and with STRATEGY_RANDOM also while sealed.
    if (request->command != CMD_ITEM && Ability_IsPsynergy(abilityId) != 0) {
      request->command = CMD_PSYNERGY;
      request->parameter = abilityId;
      if (ability->ppCost > unit->pp &&
          enemy->targetStrategy != STRATEGY_MOST_HP)
        goto next_candidate;
      if (unit->psySeal != 0 && enemy->targetStrategy == STRATEGY_RANDOM)
        goto next_candidate;
    }
    // A challenged enemy won't flee, and at its turn it no longer chooses a
    // guard (guards are settled as the round is built): both pick again.
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

    // The target. With no one listed on the side (TARGET_PICK_SIDE_EMPTY) a
    // random place is taken instead, on the actor's own side for both modes
    // (allyIsParty and opponentIsParty both test the actor); with no one worth
    // it (TARGET_PICK_NONE) the enemy picks again. A mode no ability has (3, or
    // one above TARGET_MODE_SELF) targets the user, but 3 leaves the choice
    // open, so the enemy picks again after it too.
    request->range = ability->range;
    targetMode = ability->targetMode;
    switch (targetMode) {
    case TARGET_MODE_ALLIES:
    case TARGET_MODE_SELF:
      allyTarget = BattleTarget_SelectForAction(request->unitId, ability);
      if (allyTarget == TARGET_PICK_SIDE_EMPTY) {
        allyIsParty = 0;
        if (request->unitId <= (u32)ALLY_MAX)
          allyIsParty = 1;
        allyTarget = BattleTarget_SelectRandomPosition(allyIsParty);
      }
      if (allyTarget != TARGET_PICK_NONE) {
        request->target = allyTarget;
        needsChoice = 0;
      }
      break;
    case TARGET_MODE_FOES:
      opponentTarget = BattleTarget_SelectForAction(request->unitId, ability);
      if (opponentTarget == TARGET_PICK_SIDE_EMPTY) {
        opponentIsParty = 0;
        if (request->unitId <= (u32)ALLY_MAX)
          opponentIsParty = 1;
        opponentTarget = BattleTarget_SelectRandomPosition(opponentIsParty);
      }
      if (opponentTarget != TARGET_PICK_NONE) {
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
  // As the round is built, one pick is all.
  if (retry == 0)
    needsChoice = 0;
next_candidate:
  // On the 18th failed pick, defend.
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
