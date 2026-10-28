/// @file
/// An enemy's choice of action, BattleAI_ChooseAction.
// Part of battle.c.

#include "../battle_internal.h"

/// An enemy's memory of its action choice, kept in Unit.aiActionIndex (0 in
/// every row of enemies.c, so it starts cleared). Only
/// ACTION_STRATEGY_RANDOM_START and ACTION_STRATEGY_IN_ORDER use it.
/// bits: 31..4 unused | 3..1 selection | 0 initialized
struct BattleAIState {
  unsigned initialized
      : 1;                 ///< ACTION_STRATEGY_RANDOM_START has drawn its start
  unsigned selection : 3;  ///< the place in its action list, 0 to 7
  unsigned remaining : 28; ///< not used here
};

/// Chooses an enemy's action (a party member's is chosen in the menu): one of
/// the eight in its definition, picked by its action strategy, with an item
/// instead where its item mask says so, and a target worth the ability. A
/// choice that cannot be made (too little PP, sealed, a forbidden flight, no
/// worthwhile target) picks again; after 18 tries the enemy defends.
/// Called with retry 0 for each enemy as the turn is built
/// (Battle_BuildOpponentActions), and with retry 1 when it acts
/// (BattleStaging_DispatchAction). Writes request's command, parameter,
/// target and range; ACTION_STRATEGY_RANDOM_START and
/// ACTION_STRATEGY_IN_ORDER advance the place remembered in
/// Unit.aiActionIndex. Returns nothing.
/// @param retry 0 when the turn is built: only a guard is settled now (it
/// must raise before anyone acts); 1 when the enemy acts: the rest
#ifdef UBFIX
void BattleAI_ChooseAction(struct BattleAction *request, s32 retry) {
#else
s32 BattleAI_ChooseAction(struct BattleAction *request, s32 retry) {
#endif
  u32 selection;
  struct BattleAIState *selectionMemory;
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
  struct AbilityDefinition *itemAction;
  struct AbilityDefinition *ability;
  struct Unit *unit;
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

  unit = Unit_GetFar(request->unitId);
  needsChoice = itemsAllowed = 1;
  failedChoices = 0;
  // ACTION_STRATEGY_NEXT_PICK's first pick is place 0, and
  // ACTION_STRATEGY_SAME would keep this -1 as its place; no row of
  // enemies.c uses either.
  selection = -1U;
  // A party member chooses in the menu.
  if (unit->classId != CLASS_NONE)
    return;
  if (retry != 0 && request->command != COMMAND_ENEMY_ABILITY)
    return;

  enemy = (struct EnemyDefinition *)Enemy_GetDefinitionFar(unit->id);
  selectionMemory = (struct BattleAIState *)&unit->aiActionIndex;
  actionStrategy = &enemy->actionStrategy;
  itemActionMask = &enemy->itemActions;

  // A place in the action list, by the enemy's strategy.
select_again:
  strategyKind = *actionStrategy;
  switch (strategyKind) {
  // The three weight tables are in battle_data.s.
  case ACTION_STRATEGY_EVEN:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleAI_ActionWeights[ACTION_STRATEGY_EVEN]);
    break;
  case ACTION_STRATEGY_FALLING:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleAI_ActionWeights[ACTION_STRATEGY_FALLING]);
    break;
  case ACTION_STRATEGY_FIRST_FAVOURED:
    selection = BattleEnemy_SelectWeightedAction(
        gBattleAI_ActionWeights[ACTION_STRATEGY_FIRST_FAVOURED]);
    break;
  case ACTION_STRATEGY_RANDOM_START:
    if (selectionMemory->initialized == 0) {
      selectionMemory->selection = Battle_RandomFar();
      selectionMemory->initialized = 1;
    }
    // fall through
  case ACTION_STRATEGY_IN_ORDER:
    // It moves on only when the enemy acts, wrapping from 7 to 0 in its three
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
  abilityId = enemy->abilities[selection];
  request->command = COMMAND_ENEMY_ABILITY;
  if (useItem != 0 && retry != 0) {
    if (!(INVENTORY_ITEM_ID_MASK & unit->inventory[0])) {
      useItem = 0;
      if (enemy->targetStrategy == TARGET_STRATEGY_MOST_HP) {
        request->command = COMMAND_ITEM;
        // The parameter is the ability Can't Use, yet COMMAND_ITEM reads its
        // parameter as an inventory slot (BattleActionRun_Build): slot 509,
        // 1,018 bytes past the unit's inventory. For the first three enemies
        // that is the twelfth inventory slot of the enemy three places on,
        // which no enemy fills, so the item is the empty one and the enemy
        // defends (MsgActorDefends); for the others it lies past the enemy
        // units.
        // XXX: what a fourth to sixth enemy's action shows; a Brigand or Rat
        // Fighter (TARGET_STRATEGY_MOST_HP, item places) in such a place, out
        // of items, would show it.
        request->parameter = ABILITY_CANT_USE;
        return;
      }
    }
    if (useItem != 0) {
      item = Item_GetDirectFar(unit->inventory[0]);
      // Only an item that is used up, whose ability's flags are exactly
      // ABILITY_FORMULA_HEAL_HP or ABILITY_FORMULA_EFFECT, with no higher bit
      // set.
      if (item->useType == ITEM_USE_CONSUMED) {
        itemAction = Ability_Get(item->abilityId);
        abilityId = item->abilityId;
        request->command = COMMAND_ITEM;
        flags = itemAction->flags;
        request->parameter = 0;
        switch (flags) {
        case ABILITY_FORMULA_HEAL_HP:
        case ABILITY_FORMULA_EFFECT:
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
    // A guard or a flight becomes its own command, aimed at the user itself.
    ability = Ability_Get(abilityId);
    effect = ability->effect;
    switch (effect) {
    case ABILITY_EFFECT_DEFEND:
      request->command = COMMAND_DEFEND;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_TOTAL_DEFENSE:
      request->command = COMMAND_TOTAL_DEFENSE;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    case ABILITY_EFFECT_FLEE:
      request->command = COMMAND_FLEE;
      request->target = Battle_FindTaggedSlotByValue(request->unitId);
      break;
    }

    // As the turn is built, only a guard is kept; anything else waits for
    // the enemy's action.
    if (retry == 0) {
      switch (request->command) {
      case COMMAND_DEFEND:
      case COMMAND_TOTAL_DEFENSE:
        break;
      default:
        goto done;
      }
    }

    // Psynergy: an enemy whose strategy is not TARGET_STRATEGY_MOST_HP avoids
    // it without the PP, and with TARGET_STRATEGY_RANDOM also while sealed.
    if (request->command != COMMAND_ITEM &&
        Ability_IsPsynergy(abilityId) != 0) {
      request->command = COMMAND_PSYNERGY;
      request->parameter = abilityId;
      if ((ability->ppCost > unit->pp &&
           enemy->targetStrategy != TARGET_STRATEGY_MOST_HP) ||
          (unit->psySeal != 0 &&
           enemy->targetStrategy == TARGET_STRATEGY_RANDOM))
        goto next_candidate;
    }
    // A challenged enemy won't flee, and when it acts it no longer chooses a
    // guard (guards are settled as the turn is built): both pick again.
    command = request->command;
    if ((command == COMMAND_FLEE && (u8)unit->challenge != 0) ||
        (retry != 0 &&
         (command == COMMAND_DEFEND || command == COMMAND_TOTAL_DEFENSE)))
      goto next_candidate;

    if (command == COMMAND_ENEMY_ABILITY) {
      request->parameter = abilityId;
      if (abilityId == ABILITY_ATTACK)
        request->command = COMMAND_ATTACK;
    }

    // The target. With no one listed on the side (TARGET_PICK_SIDE_EMPTY) a
    // random place is taken instead, on the unit's own side for both modes
    // (allyIsParty and opponentIsParty both test the unit); with no one worth
    // it (TARGET_PICK_NONE) the enemy picks again. A mode no ability has (3, or
    // one above ABILITY_TARGET_SELF) targets the user, but 3 leaves the choice
    // open, so the enemy picks again after it too.
    request->range = ability->range;
    targetMode = ability->targetMode;
    switch (targetMode) {
    case ABILITY_TARGET_ALLIES:
    case ABILITY_TARGET_SELF:
      allyTarget = BattleAI_ChooseTarget(request->unitId, ability);
      if (allyTarget == TARGET_PICK_SIDE_EMPTY) {
        allyIsParty = 0;
        if (request->unitId <= (u32)ALLY_MAX)
          allyIsParty = 1;
        allyTarget = BattleAI_ChooseRandomTarget(allyIsParty);
      }
      if (allyTarget != TARGET_PICK_NONE) {
        request->target = allyTarget;
        needsChoice = 0;
      }
      break;
    case ABILITY_TARGET_FOES:
      opponentTarget = BattleAI_ChooseTarget(request->unitId, ability);
      if (opponentTarget == TARGET_PICK_SIDE_EMPTY) {
        opponentIsParty = 0;
        if (request->unitId <= (u32)ALLY_MAX)
          opponentIsParty = 1;
        opponentTarget = BattleAI_ChooseRandomTarget(opponentIsParty);
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
  // As the turn is built, one pick is all.
  if (retry == 0)
    needsChoice = 0;
next_candidate:
  // On the 18th failed pick, defend.
  if (needsChoice != 0 && failedChoices > 16) {
    request->command = COMMAND_DEFEND;
    needsChoice = 0;
  }
  failedChoices++;
  if (needsChoice != 0)
    goto select_again;
done:
  return;
}
