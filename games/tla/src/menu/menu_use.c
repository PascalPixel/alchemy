/* Using an item or Psynergy from the menu: what it does to the party, and the
   sound it plays. Part of menu.c. */

/* Uses the item in memberId's inventory slot on tgtId, or on the whole party
   when its ability's range is ABILITY_RANGE_ALL. An item heals by its ability's
   plain power, not by its user's elemental power as Psynergy does. A consumed
   item leaves the bag and Hermes' Water leaves its Empty Bottle; returns -1
   when it did nothing, keeping the item. */
Sint32 mnItemUse(Sint32 slot, Sint32 memberId, Sint32 tgtId)
{
	ITEM_MENU_STATE *menu;
	Sint32			 useResult;
	Sint32			 itemId;
	UNIT			*unit;
	ITEM_DEFINITION *item;

	unit = syUnitGet(memberId);
	menu = RamHeapSlots->menuState;
	itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
	item = gmItemGetDirectStub(itemId);
	useResult = mnAbilityApplyToTargets(
		PSYNERGY_ABILITY_MASK & item->abilityId, memberId, tgtId, 1);
	if(useResult != -1) {
		item = gmItemGetDirectStub(unit->inventory[slot]);
		if(item->useType == ITEM_USE_CONSUMED) {
			gmInventoryRemoveStub(memberId, slot);
			menu->itemCnt = mnItemMenuCollectItems(unit, menu->items, 0);
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

Sint32 gmBattleCalcRecoveryStub(
	Sint32 value, Sint32 selector, Sint32 multiplier);

/* Applies the healing, curing, revival or lasting stat rise of ability
   abilityId, used by party member userId, to tgtId, or to the whole party
   when its range is ABILITY_RANGE_ALL. HP healing is scaled by the user's power
   in the ability's element unless plainPower is set, as it is for an item.
   Leaves the result's message in the menu's state; returns 0, or -1 when
   nothing changed. */
Sint32 mnAbilityApplyToTargets(
	Sint32 abilityId, Sint32 userId, Sint32 tgtId, Sint32 plainPower)
{
	ABILITY_DEFINITION *ability;
	ITEM_MENU_STATE	   *menu;
	UNIT			   *tgt;
	UNIT			   *caster;
	Sint32				partyMsgChosen;
	Sint32				changed;
	Uint8				tgtIdx;
	Sint16				elementPower;
	Sint32				randomAdjust;
	Sint32				amount;
	Sint32				result;

	menu = RamHeapSlots->menuState;
	ability = gmAbilityGetDirectStub(abilityId);
	changed = 0;
	result = 0;
	partyMsgChosen = 0;

	if(tgtId == MENU_TARGET_WHOLE_PARTY)
		tgt = syUnitGet(MainRoutineStateWork.leaderActorId);
	else
		tgt = syUnitGet(tgtId);

	/* FAKEMATCH(dead): the dead test loads ability->range ahead of the count
	 * guard. */
	if(ability->range == ABILITY_RANGE_ALL && partyMsgChosen != 0)
		result = ITEM_USE_RESULT_PARTY_HP_RECOVERED;

	for(tgtIdx = 0; tgtIdx < menu->partyCnt; tgtIdx++) {
		if(ability->range == ABILITY_RANGE_ALL) {
			tgtId = (Uint16)menu->memberIds[tgtIdx];
			tgt = syUnitGet(tgtId);
		}

		amount = ability->power;
		switch(ability->flg & ABILITY_FORMULA_MASK) {
		case ABILITY_FORMULA_HEAL_HP:
			if(plainPower == 0) {
				if(ability->element != ELEMENT_NONE) {
					Sint32 elementIdx;

					caster = syUnitGet(userId);
					elementIdx = ability->element;
					elementPower = caster->element[elementIdx].power;
				} else
					elementPower = HEALING_BASE_POWER;
				amount = gmBattleCalcRecoveryStub(
					amount, elementPower, BATTLE_MULTIPLIER_ONE);
			}

			if(tgt->hp <= 0) {
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_HP_NOT_RECOVERED;
			} else {
				if(tgt->hp == tgt->hpMax) {
					if(partyMsgChosen == 0)
						result = ITEM_USE_RESULT_HP_FULL;
				} else {
					tgt->hp += amount;
					if(tgt->hp > tgt->hpMax) {
						amount -= tgt->hp - tgt->hpMax;
						tgt->hp = tgt->hpMax;
						if(partyMsgChosen == 0)
							result = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
					} else if(partyMsgChosen == 0) {
						result = ITEM_USE_RESULT_HP_RECOVERED;
					}
					gmUnitRecalculateGaugesStub(tgtId);
					changed = 1;
					if(ability->range == ABILITY_RANGE_ALL) {
						partyMsgChosen = 1;
						result = ITEM_USE_RESULT_PARTY_HP_RECOVERED;
					}
				}
			}
			break;

		/* The lasting rises: the item's power, give or take one, except Lucky
		   Pepper's. One time in four it is one more, one time in four one
		   less. */
		case ABILITY_FORMULA_NO_POWER:
			randomAdjust = syRandomBelow(4);
			if(randomAdjust == 0) {
				randomAdjust = -1;
			} else {
				if(randomAdjust == 1)
					randomAdjust = 1;
				else
					randomAdjust = 0;
			}

			switch(abilityId & PSYNERGY_ABILITY_MASK) {
			case ABILITY_POWER_BREAD:
				tgt->baseHp += amount + randomAdjust;
				result = ITEM_USE_RESULT_MAX_HP_UP;
				changed = 1;
				break;
			case ABILITY_COOKIE:
				tgt->basePp += amount + randomAdjust;
				result = ITEM_USE_RESULT_MAX_PP_UP;
				changed = 1;
				break;
			case ABILITY_MINT:
				tgt->baseAgility += amount + randomAdjust;
				result = ITEM_USE_RESULT_AGILITY_UP;
				changed = 1;
				break;
			case ABILITY_LUCKY_PEPPER:
				tgt->baseLuck += amount;
				result = ITEM_USE_RESULT_LUCK_UP;
				changed = 1;
				break;
			case ABILITY_APPLE:
				tgt->baseAttack += amount + randomAdjust;
				/* "Your Attack increased!" shows no number: neither this 3 nor
				   the Apple's power is seen. */
				mnMessageArgumentPushStub(3, MESSAGE_ARGUMENT_NUMBER);
				result = ITEM_USE_RESULT_ATTACK_UP;
				changed = 1;
				break;
			case ABILITY_HARD_NUT:
				tgt->baseDefense += amount + randomAdjust;
				/* Nor does "Your Defense increased!" show this 4. */
				mnMessageArgumentPushStub(4, MESSAGE_ARGUMENT_NUMBER);
				result = ITEM_USE_RESULT_DEFENSE_UP;
				changed = 1;
				break;
			}
			break;

		case ABILITY_FORMULA_HEAL_PP:
			if(tgt->pp == tgt->ppMax) {
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_PP_FULL;
			} else {
				tgt->pp += amount;
				if(tgt->pp > tgt->ppMax) {
					amount -= tgt->pp - tgt->ppMax;
					tgt->pp = tgt->ppMax;
					if(partyMsgChosen == 0)
						result = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
				} else if(partyMsgChosen == 0) {
					result = ITEM_USE_RESULT_PP_RECOVERED;
				}
				gmUnitRecalculateGaugesStub(tgtId);
				changed = 1;
				if(ability->range == ABILITY_RANGE_ALL) {
					partyMsgChosen = 1;
					result = ITEM_USE_RESULT_PARTY_PP_RECOVERED;
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

		switch(ability->eff) {
		case ABILITY_EFFECT_RESTORE_HP:
			if(tgt->hp <= 0 || tgt->hp == tgt->hpMax) {
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_HP_NOT_RECOVERED;
			} else {
				tgt->hp += amount;
				if(tgt->hp > tgt->hpMax) {
					tgt->hp = tgt->hpMax;
					if(partyMsgChosen == 0)
						result = ITEM_USE_RESULT_HP_FULLY_RECOVERED;
				} else if(partyMsgChosen == 0) {
					result = ITEM_USE_RESULT_HP_RECOVERED;
				}
				gmUnitRecalculateGaugesStub(tgtId);
				changed = 1;
			}
			break;

		case ABILITY_EFFECT_RESTORE_PP:
			if(tgt->pp == tgt->ppMax) {
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_PP_FULL;
			} else {
				tgt->pp += amount;
				if(tgt->pp > tgt->ppMax) {
					tgt->pp = tgt->ppMax;
					if(partyMsgChosen == 0)
						result = ITEM_USE_RESULT_PP_FULLY_RECOVERED;
				} else if(partyMsgChosen == 0) {
					result = ITEM_USE_RESULT_PP_RECOVERED;
				}
				gmUnitRecalculateGaugesStub(tgtId);
				changed = 1;
			}
			break;

		case ABILITY_EFFECT_REVIVE:
			if(tgt->hp == 0) {
				tgt->hp = tgt->hpMax;
				gmUnitRecalculateGaugesStub(tgtId);
				changed = 1;
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_REVIVED;
			} else if(partyMsgChosen == 0) {
				result = ITEM_USE_RESULT_NOT_DOWN;
			}
			break;

		/* Quartz's and Dew's revivals, which only battle uses: the menu never
		   reaches these two. */
		case ABILITY_EFFECT_REVIVE_HALF:
			if(tgt->hp == 0) {
				tgt->hp = tgt->hpMax / 2;
				gmUnitRecalculateGaugesStub(tgtId);
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_REVIVED;
			} else if(partyMsgChosen == 0) {
				result = ITEM_USE_RESULT_NOT_DOWN;
			}
			break;

		case ABILITY_EFFECT_DEW:
			if(tgt->hp == 0) {
				tgt->hp = tgt->hpMax * 7 / 10;
				gmUnitRecalculateGaugesStub(tgtId);
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_REVIVED;
			} else if(partyMsgChosen == 0) {
				result = ITEM_USE_RESULT_NOT_DOWN;
			}
			break;

		case ABILITY_EFFECT_CURE_POISON:
			if(tgt->poison != 0) {
				tgt->poison = 0;
				changed = 1;
				if(partyMsgChosen == 0)
					result = ITEM_USE_RESULT_POISON_PURGED;
			} else if(partyMsgChosen == 0) {
				result = ITEM_USE_RESULT_NOT_POISONED;
			}
			break;
		}

		if(ability->range != ABILITY_RANGE_ALL)
			break;
	}

	if(changed == 0) {
		menu->msgOfs = result;
		return -1;
	}

	for(tgtIdx = 0; tgtIdx < menu->partyCnt; tgtIdx++)
		gmUnitCalcStub((Uint16)menu->memberIds[tgtIdx]);
	menu->msgOfs = result;
	return 0;
}

/* Plays the sound of using item from the menu (mnAbilityPlayUseSnd), after a
   use that changed something. */
void mnItemPlayUseSnd(Sint32 item)
{
	mnAbilityPlayUseSnd(
		gmItemGetDirectStub(item)->abilityId & PSYNERGY_ABILITY_MASK);
}

/* Plays the sound of a Psynergy or item used from the menu; unlike in battle,
   no animation is drawn and nothing waits. Healing HP or PP plays SE_RECOVERY,
   revival MUS_RECOVERING_AFTER_COMBAT, curing poison MUS_POISON_CURED, and any
   other effect MUS_LUCKY. */
void mnAbilityPlayUseSnd(Sint32 abilityId)
{
	Sint32				formula;
	Uint32				effect;
	ABILITY_DEFINITION *ability;

	ability = gmAbilityGetDirectStub(abilityId);
	formula = ABILITY_FORMULA_MASK & ability->flg;
	switch(formula) {
	case ABILITY_FORMULA_HEAL_HP:
		/* Started twice on the same music player, the second start restarts the
		   first in the same tick, so it is heard once. */
		mnSndPlayReturnOne(SE_RECOVERY);
		/* Fall through. */
	case ABILITY_FORMULA_HEAL_PP:
		mnSndPlayReturnOne(SE_RECOVERY);
		return;
	default:
		effect = ability->eff;
		switch(effect) {
		case ABILITY_EFFECT_REVIVE:
			mnSndPlayReturnOne(MUS_RECOVERING_AFTER_COMBAT);
			return;
		case ABILITY_EFFECT_CURE_POISON:
			mnSndPlayReturnOne(MUS_POISON_CURED);
			return;
		default:
			mnSndPlayReturnOne(MUS_LUCKY);
			/* Fall through. */

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
