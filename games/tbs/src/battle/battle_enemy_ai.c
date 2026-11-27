/* An enemy's choice of action, btAIChooseAction. Part of battle.c. */

#include "battle_internal.h"

/* The choices an enemy may fail to carry out before it defends instead. */
#define AI_MAX_FAILED_CHOICES 16

/* Chooses an enemy's action: one of the eight in its definition, picked by its
   strategy, and a target worth it; if a pick fails after more than
   AI_MAX_FAILED_CHOICES have failed before it, the enemy defends. acting is 0
   as the turn is built, when only a guard is chosen, and 1 when the enemy
   acts. */
UNREAD_RESULT btAIChooseAction(BATTLE_ACTION *request, Sint32 acting)
{
	Uint32					selection;
	BATTLE_AI_STATE		   *selectionMemory;
	const ENEMY_DEFINITION *enemy;
	Sint32					itemsAllowed;
	Sint32					failedChoices;
	Sint32					needsChoice;
	const Sint8			   *actionStrategy;
	const Uint8			   *itemActionMask;
	Sint16					command;
	Sint32					allyTarget;
	Sint32					opponentTarget;
	Sint32					allyIsParty;
	Sint32					opponentIsParty;
	ITEM_DEFINITION		   *item;
	Sint32					useItem;
	Sint8					strategyKind;
	ABILITY_DEFINITION	   *itemAction;
	ABILITY_DEFINITION	   *ability;
	UNIT				   *unit;
	Sint32					abilityId;
	Sint32					flags;
	Uint8					effect;
	Uint8					targetMode;

	/* Picks a place in the action list from a 0-255 roll and eight weights: the
	   first place whose running total passes the roll, else place 0. */
	Sint32 btEnemySelectWeightedAction(const Uint8 *weights)
	{
		Sint32 roll;
		Sint32 total;
		Sint32 selectedAction;
		Sint32 actionIndex;

		roll = gmBattleRandomStub() & 0xff;
		total = weights[0];
		selectedAction = 0;
		actionIndex = 0;
		if(roll >= total) {
			while(++actionIndex <= 7) {
				total += weights[actionIndex];
				if(roll < total) {
					selectedAction = actionIndex;
					break;
				}
			}
		}
		return selectedAction;
	}

	unit = gmUnitGetStub(request->unitId);
	needsChoice = itemsAllowed = 1;
	failedChoices = 0;
	/* No enemy uses ACTION_STRATEGY_NEXT_PICK or ACTION_STRATEGY_SAME. */
	selection = -1U;
	/* A party member chooses in the menu. */
	if(unit->classId != CLASS_NONE)
		return;
	if(acting != 0 && request->command != COMMAND_ENEMY_ABILITY)
		return;

	enemy = gmEnemyGetDefinitionStub(unit->id);
	selectionMemory = &unit->aiState;
	actionStrategy = &enemy->actionStrategy;
	itemActionMask = &enemy->itemActions;

	/* A place in the action list, by the enemy's strategy. */
select_again:
	strategyKind = *actionStrategy;
	switch(strategyKind) {
	/* The three weight tables are in battle_tables.c. */
	case ACTION_STRATEGY_EVEN:
		selection = btEnemySelectWeightedAction(
			BattleAIActionWeights[ACTION_STRATEGY_EVEN]);
		break;
	case ACTION_STRATEGY_FALLING:
		selection = btEnemySelectWeightedAction(
			BattleAIActionWeights[ACTION_STRATEGY_FALLING]);
		break;
	case ACTION_STRATEGY_FIRST_FAVOURED:
		selection = btEnemySelectWeightedAction(
			BattleAIActionWeights[ACTION_STRATEGY_FIRST_FAVOURED]);
		break;
	case ACTION_STRATEGY_RANDOM_START:
		if(selectionMemory->initialized == 0) {
			selectionMemory->selection = gmBattleRandomStub();
			selectionMemory->initialized = 1;
		}
		/* fall through */
	case ACTION_STRATEGY_IN_ORDER:
		/* It moves on only when the enemy acts, wrapping from 7 to 0 in its
		   three bits. */
		selection = selectionMemory->selection;
		if(acting != 0) {
			selectionMemory->selection = selection + 1;
		}
		break;
	case ACTION_STRATEGY_NEXT_PICK:
		selection++;
		break;
	case ACTION_STRATEGY_SAME:
		break;
	}

	/* A place marked for an item: use the first item held, if any. Once an item
	   is refused, later picks take the place's ability instead. */
	useItem = (*itemActionMask >> selection) & 1;
	useItem &= itemsAllowed;
	abilityId = enemy->abilities[selection];
	request->command = COMMAND_ENEMY_ABILITY;
	if(useItem != 0 && acting != 0) {
		if(!(INVENTORY_ITEM_ID_MASK & unit->inventory[0])) {
			useItem = 0;
			if(enemy->targetStrategy == TARGET_STRATEGY_MOST_HP) {
				request->command = COMMAND_ITEM;
				/* BUG: the ability Can't Use, read by the item command as
				   inventory slot 509; the enemy defends (README.md, "The
				   party's commands"). */
				request->param = ABILITY_CANT_USE;
				return;
			}
		}
		if(useItem != 0) {
			item = gmItemGetDirectStub(unit->inventory[0]);
			/* Only an item that is used up and heals or has an effect. */
			if(item->useType == ITEM_USE_CONSUMED) {
				itemAction = gmAbilityGetDirectStub(item->abilityId);
				abilityId = item->abilityId;
				request->command = COMMAND_ITEM;
				flags = itemAction->flags;
				request->param = 0;
				switch(flags) {
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
		if(useItem == 0)
			itemsAllowed = 0;
	}

	if(needsChoice != 0) {
		/* A guard or a flight becomes its own command, aimed at the user
		   itself. */
		ability = gmAbilityGetDirectStub(abilityId);
		effect = ability->effect;
		switch(effect) {
		case ABILITY_EFFECT_DEFEND:
			request->command = COMMAND_DEFEND;
			request->target = btFindUnitPlace(request->unitId);
			break;
		case ABILITY_EFFECT_TOTAL_DEFENSE:
			request->command = COMMAND_TOTAL_DEFENSE;
			request->target = btFindUnitPlace(request->unitId);
			break;
		case ABILITY_EFFECT_FLEE:
			request->command = COMMAND_FLEE;
			request->target = btFindUnitPlace(request->unitId);
			break;
		}

		/* As the turn is built, only a guard is kept; anything else waits for
		   the enemy's action. */
		if(acting == 0) {
			switch(request->command) {
			case COMMAND_DEFEND:
			case COMMAND_TOTAL_DEFENSE:
				break;
			default:
				goto done;
			}
		}

		/* Psynergy: an enemy whose strategy is not TARGET_STRATEGY_MOST_HP
		   avoids it without the PP, and with TARGET_STRATEGY_RANDOM also while
		   sealed. */
		if(request->command != COMMAND_ITEM &&
			btAbilityIsPsynergy(abilityId) != 0) {
			request->command = COMMAND_PSYNERGY;
			request->param = abilityId;
			if((ability->ppCost > unit->pp &&
				   enemy->targetStrategy != TARGET_STRATEGY_MOST_HP) ||
				(unit->psySeal != 0 &&
					enemy->targetStrategy == TARGET_STRATEGY_RANDOM))
				goto next_candidate;
		}
		/* A challenged enemy won't flee, and when it acts it no longer chooses
		   a guard (guards are settled as the turn is built): both pick
		   again. */
		command = request->command;
		if((command == COMMAND_FLEE && (Uint8)unit->challenge != 0) ||
			(acting != 0 &&
				(command == COMMAND_DEFEND ||
					command == COMMAND_TOTAL_DEFENSE)))
			goto next_candidate;

		if(command == COMMAND_ENEMY_ABILITY) {
			request->param = abilityId;
			if(abilityId == ABILITY_ATTACK)
				request->command = COMMAND_ATTACK;
		}

		/* The target: with no one on the side, a random place on the enemy's
		   own side; with no one worth it, the enemy picks again. */
		request->range = ability->range;
		targetMode = ability->targetMode;
		switch(targetMode) {
		case ABILITY_TARGET_ALLIES:
		case ABILITY_TARGET_SELF:
			allyTarget = btAIChooseTarget(request->unitId, ability);
			if(allyTarget == TARGET_PICK_SIDE_EMPTY) {
				allyIsParty = 0;
				if(request->unitId >= 0 && request->unitId <= ALLY_MAX)
					allyIsParty = 1;
				allyTarget = btAIChooseRandomTarget(allyIsParty);
			}
			if(allyTarget != TARGET_PICK_NONE) {
				request->target = allyTarget;
				needsChoice = 0;
			}
			break;
		case ABILITY_TARGET_FOES:
			opponentTarget = btAIChooseTarget(request->unitId, ability);
			if(opponentTarget == TARGET_PICK_SIDE_EMPTY) {
				opponentIsParty = 0;
				if(request->unitId >= 0 && request->unitId <= ALLY_MAX)
					opponentIsParty = 1;
				opponentTarget = btAIChooseRandomTarget(opponentIsParty);
			}
			if(opponentTarget != TARGET_PICK_NONE) {
				request->target = opponentTarget;
				needsChoice = 0;
			}
			break;
		case ABILITY_TARGET_UNUSED:
			request->target = btFindUnitPlace(request->unitId);
			break;
		default:
			opponentTarget = btFindUnitPlace(request->unitId);
			request->target = opponentTarget;
			needsChoice = 0;
			break;
		}
	}
	/* As the turn is built, one pick is all. */
	if(acting == 0)
		needsChoice = 0;
next_candidate:
	if(needsChoice != 0 && failedChoices > AI_MAX_FAILED_CHOICES) {
		request->command = COMMAND_DEFEND;
		needsChoice = 0;
	}
	failedChoices++;
	if(needsChoice != 0)
		goto select_again;
done:
	return;
}

#undef AI_MAX_FAILED_CHOICES
