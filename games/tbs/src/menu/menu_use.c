/* Using an item or Psynergy from the menu: what it does to the party, and the
   sound it plays. Part of menu.c. */

#include "menu_internal.h"

/* Lends HIGHLIGHT_PALETTE to the item icons and makes the highlight in the
   palette below it instead. Nothing calls it. */
void mnItemPrepareUsePalette(void)
{
	mnPaletteLoadIconsIntoHighlight();
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE - 1);
}

/* Unused: does nothing. */
UNUSED void mnItemUseNoOperation(void)
{
}

/* Uses the item in memberId's inventory slot on targetId, or on every party
   member when its action's range is ABILITY_RANGE_ALL: applies its action at
   its plain power (mnAbilityApplyToTargets with fixedScale 1). */
Sint32 mnItemUse(Sint32 slot, Sint32 memberId, Sint32 targetId)
{
	ITEM_MENU_STATE *menu;
	Sint32			 useResult;
	Sint32			 itemId;
	UNIT			*unit;
	ITEM_DEFINITION *item;

	unit = gmUnitGetStub(memberId);
	menu = MenuState;
	itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
	item = gmItemGetDirectStub(itemId);
	useResult = mnAbilityApplyToTargets(
		PSYNERGY_ABILITY_MASK & item->abilityId, memberId, targetId, 1);
	if(useResult != -1) {
		item = gmItemGetDirectStub(unit->inventory[slot]);
		if(item->useType == ITEM_USE_CONSUMED) {
			gmInventoryRemoveStub(memberId, slot);
			menu->itemCount = mnItemMenuCollectItems(unit, menu->items, 0);
		}
		if(item->useType == ITEM_USE_EMPTIES) {
			if(itemId == ITEM_HERMES_WATER)
				itemId = ITEM_EMPTY_BOTTLE;
			unit->inventory[slot] = itemId;
		}
		useResult = 0;
	}
	return useResult;
}

/* Unused: returns 1. */
UNUSED Sint32 mnItemUseReturnTrue(void)
{
	return 1;
}

/* Applies the healing, curing, revival or lasting stat rise of action effectId,
   used by party member srcId, to targetId, or to every party member when the
   action's range is ABILITY_RANGE_ALL. */
Sint32 mnAbilityApplyToTargets(
	Sint32 effectId, Sint32 srcId, Sint32 targetId, Sint32 fixedScale)
{
	ABILITY_DEFINITION *effect;
	ITEM_MENU_STATE	   *menu;
	UNIT			   *target;
	UNIT			   *caster;
	Sint32				partyResultShown;
	Sint32				changed;
	Uint8				targetIndex;
	Sint16				elementPower;
	Sint32				randomAdjust;
	Sint32				amount;
	Sint32				msgOffset;

	effect = gmAbilityGetDirectStub(effectId);
	menu = MenuState;
	changed = 0;
	msgOffset = 0;
	partyResultShown = 0;

	if(targetId != MENU_TARGET_WHOLE_PARTY)
		target = gmUnitGetStub(targetId);
	else
		target = gmUnitGetStub(ISAAC);

	/* FAKEMATCH(dead): a test that is never true (partyResultShown is still
	 * 0). */
	if(effect->range == ABILITY_RANGE_ALL && partyResultShown != 0)
		msgOffset = ITEM_USE_RESULT_PARTY_HP_RECOVERED;

	/* A party-wide action keeps the first useful message while visiting
	   targets. */
	for(targetIndex = 0; targetIndex < menu->partyCount; targetIndex++) {
		if(effect->range == ABILITY_RANGE_ALL) {
			targetId = menu->memberIds[targetIndex];
			target = gmUnitGetStub(targetId);
		}

		/* The ability kind first; the result messages are ITEM_USE_RESULT. */
		amount = effect->power;
		switch(effect->flags & ABILITY_FORMULA_MASK) {
		case ABILITY_FORMULA_HEAL_HP:
			if(fixedScale == 0) {
				if(effect->element != ELEMENT_NONE) {
					caster = gmUnitGetStub(srcId);
					elementPower = caster->element[effect->element].power;
				} else
					elementPower = HEALING_BASE_POWER;
				amount = gmBattleCalculateRecoveryStub(
					amount, elementPower, BATTLE_MULTIPLIER_ONE);
			}

			if(target->hp <= 0) {
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_HP_NOT_RECOVERED;
			} else {
				if(target->hp == target->hpMax) {
					if(partyResultShown == 0)
						msgOffset = ITEM_USE_RESULT_HP_FULL;
				} else {
					target->hp += amount;
					if(target->hp > target->hpMax) {
						amount -= target->hp - target->hpMax;
						target->hp = target->hpMax;
						if(partyResultShown == 0)
							msgOffset = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
					} else if(partyResultShown == 0) {
						msgOffset = ITEM_USE_RESULT_HP_RECOVERED;
					}
					gmUnitRecalculateGaugesStub(targetId);
					changed = 1;
					if(effect->range == ABILITY_RANGE_ALL) {
						partyResultShown = 1;
						msgOffset = ITEM_USE_RESULT_PARTY_HP_RECOVERED;
					}
				}
			}
			break;

		/* The stat items. */
		case ABILITY_FORMULA_NO_POWER:
			/* The rise varies by -1, 0 (half the time) or 1. */
			randomAdjust = syRandom16Below(4);
			if(randomAdjust == 0)
				randomAdjust = -1;
			else if(randomAdjust == 1)
				randomAdjust = 1;
			else
				randomAdjust = 0;

			switch(effectId & PSYNERGY_ABILITY_MASK) {
			case ABILITY_POWER_BREAD:
				target->baseHp += amount + randomAdjust;
				msgOffset = ITEM_USE_RESULT_MAX_HP_UP;
				changed = 1;
				break;
			case ABILITY_COOKIE:
				target->basePp += amount + randomAdjust;
				msgOffset = ITEM_USE_RESULT_MAX_PP_UP;
				changed = 1;
				break;
			case ABILITY_MINT:
				target->baseAgility += amount + randomAdjust;
				msgOffset = ITEM_USE_RESULT_AGILITY_UP;
				changed = 1;
				break;
			case ABILITY_LUCKY_PEPPER:
				target->baseLuck += amount;
				msgOffset = ITEM_USE_RESULT_LUCK_UP;
				changed = 1;
				break;
			/* No edition's "Your Attack increased!" or "Your Defense
			   increased!" shows a number, so the 3 and the 4 pushed for them
			   are never seen. */
			case ABILITY_APPLE:
				target->baseAttack += amount + randomAdjust;
				mnMessageArgumentPushStub(3, MESSAGE_ARGUMENT_NUMBER);
				msgOffset = ITEM_USE_RESULT_ATTACK_UP;
				changed = 1;
				break;
			case ABILITY_HARD_NUT:
				target->baseDefense += amount + randomAdjust;
				mnMessageArgumentPushStub(4, MESSAGE_ARGUMENT_NUMBER);
				msgOffset = ITEM_USE_RESULT_DEFENSE_UP;
				changed = 1;
				break;
			}
			break;

		case ABILITY_FORMULA_HEAL_PP:
			if(target->pp == target->ppMax) {
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_PP_FULL;
			} else {
				target->pp += amount;
				if(target->pp > target->ppMax) {
					amount -= target->pp - target->ppMax;
					target->pp = target->ppMax;
					if(partyResultShown == 0)
						msgOffset = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
				} else if(partyResultShown == 0) {
					msgOffset = ITEM_USE_RESULT_PP_RECOVERED;
				}
				gmUnitRecalculateGaugesStub(targetId);
				changed = 1;
				if(effect->range == ABILITY_RANGE_ALL) {
					partyResultShown = 1;
					msgOffset = ITEM_USE_RESULT_PARTY_PP_RECOVERED;
				}
			}
			break;

		case ABILITY_FORMULA_EFFECT:
		case ABILITY_FORMULA_ATTACK:
		case ABILITY_FORMULA_ATTACK_MULTIPLIED:
		case ABILITY_FORMULA_DAMAGE:
		case ABILITY_FORMULA_ENEMY_DAMAGE:
		case ABILITY_FORMULA_EFFECT_ONLY:
		case ABILITY_FORMULA_SUMMON:
		case ABILITY_FORMULA_PP_DAMAGE:
			break;
		}

		/* Then the ability's effect (ABILITY_EFFECT). */
		switch(effect->effect) {
		case ABILITY_EFFECT_RESTORE_HP:
			if(target->hp <= 0 || target->hp == target->hpMax) {
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_HP_NOT_RECOVERED;
			} else {
				target->hp += amount;
				if(target->hp > target->hpMax) {
					target->hp = target->hpMax;
					if(partyResultShown == 0)
						msgOffset = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
				} else if(partyResultShown == 0) {
					msgOffset = ITEM_USE_RESULT_HP_RECOVERED;
				}
				gmUnitRecalculateGaugesStub(targetId);
				changed = 1;
			}
			break;

		case ABILITY_EFFECT_RESTORE_PP:
			if(target->pp == target->ppMax) {
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_PP_FULL;
			} else {
				target->pp += amount;
				if(target->pp > target->ppMax) {
					target->pp = target->ppMax;
					if(partyResultShown == 0)
						msgOffset = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
				} else if(partyResultShown == 0) {
					msgOffset = ITEM_USE_RESULT_PP_RECOVERED;
				}
				gmUnitRecalculateGaugesStub(targetId);
				changed = 1;
			}
			break;

		case ABILITY_EFFECT_REVIVE:
			if(target->hp == 0) {
				target->hp = target->hpMax;
				gmUnitRecalculateGaugesStub(targetId);
				changed = 1;
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_REVIVED;
			} else if(partyResultShown == 0) {
				msgOffset = ITEM_USE_RESULT_NOT_DOWN;
			}
			break;

		/* Quartz's and Dew's revivals set no change, so they would report -1
		   after reviving; no item has either action (item_table.c), and no
		   Djinni is used from the menus. Dew gives 7/10 of maximum HP here. */
		case ABILITY_EFFECT_REVIVE_HALF:
			if(target->hp == 0) {
				target->hp = target->hpMax / 2;
				gmUnitRecalculateGaugesStub(targetId);
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_REVIVED;
			} else if(partyResultShown == 0) {
				msgOffset = ITEM_USE_RESULT_NOT_DOWN;
			}
			break;

		case ABILITY_EFFECT_DEW:
			if(target->hp == 0) {
				target->hp = target->hpMax * 7 / 10;
				gmUnitRecalculateGaugesStub(targetId);
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_REVIVED;
			} else if(partyResultShown == 0) {
				msgOffset = ITEM_USE_RESULT_NOT_DOWN;
			}
			break;

		case ABILITY_EFFECT_CURE_POISON:
			if(target->poison != 0) {
				target->poison = 0;
				changed = 1;
				if(partyResultShown == 0)
					msgOffset = ITEM_USE_RESULT_POISON_PURGED;
			} else if(partyResultShown == 0) {
				msgOffset = ITEM_USE_RESULT_NOT_POISONED;
			}
			break;
		}

		if(effect->range != ABILITY_RANGE_ALL)
			break;
	}

	if(changed == 0) {
		menu->msgOffset = msgOffset;
		return -1;
	}

	for(targetIndex = 0; targetIndex < menu->partyCount; targetIndex++)
		gmUnitCalculateStub(menu->memberIds[targetIndex]);
	menu->msgOffset = msgOffset;
	return 0;
}

/* Plays the use sound of item's action (mnAbilityPlayUseSound). */
void mnItemPlayUseSound(Uint32 item)
{
	mnAbilityPlayUseSound(
		gmItemGetDirectStub(item)->abilityId & PSYNERGY_ABILITY_MASK);
}

/* Plays the sound of action used from the menu, and returns at once: no
   animation is drawn and nothing waits. */
void mnAbilityPlayUseSound(Sint32 action)
{
	Sint32				formula;
	Uint32				effect;
	ABILITY_DEFINITION *actionDefinition;

	actionDefinition = gmAbilityGetDirectStub(action);
	formula = ABILITY_FORMULA_MASK & actionDefinition->flags;
	switch(formula) {
	case ABILITY_FORMULA_HEAL_HP:
		/* With no break the recovery sound is started twice, both times on
		   music player 2 (SongTable's row for it), which sySoundPlay never
		   swaps for another: the second start restarts the first before the
		   sound driver mixes a tick, so it is heard once. */
		mnSoundPlayReturnOne(SE_RECOVERY);
		/* Fall through. */
	case ABILITY_FORMULA_HEAL_PP:
		mnSoundPlayReturnOne(SE_RECOVERY);
		return;
	default:
		effect = actionDefinition->effect;
		switch(effect) {
		case ABILITY_EFFECT_REVIVE:
			mnSoundPlayReturnOne(MUS_RECOVERING_AFTER_COMBAT);
			return;
		case ABILITY_EFFECT_CURE_POISON:
			mnSoundPlayReturnOne(MUS_POISON_CURED);
			return;
		default:
			mnSoundPlayReturnOne(MUS_LUCKY);

		/* The HP and PP restores no ability has (ability_table.c). */
		case ABILITY_EFFECT_RESTORE_HP:
		case ABILITY_EFFECT_RESTORE_PP:
		case ABILITY_EFFECT_DRAIN_HP:
		case ABILITY_EFFECT_DRAIN_PP:
			return;
		}
		break;
	}
}

/* Unused: returns 0. */
UNUSED Sint32 mnAbilityReturnFalse(void)
{
	return 0;
}
