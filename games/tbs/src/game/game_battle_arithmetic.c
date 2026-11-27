/* @file */
/* Battle arithmetic: element multipliers, random rolls and success rates. */
/* Part of game.c. */

/* Returns the multiplier, 256 for 1x, for an element difference (power less */
/* resistance): 256 plus 1.28 a point, the difference held to -200 to 200, its
 */
/* effect half with `halve`. */
s32 gmBattleGetElementMultiplier(s32 difference, s32 halve) {
	s32 minimumDifference;
	s32 maximumDifference;
	s32 pointIndex;
	s32 multiplier;
	s32 differenceSpan;
	s32 multiplierSpan;

	maximumDifference = Element_PowerResistCurve[0][0];
	minimumDifference = Element_PowerResistCurve[4][0];
	{
		s32 pointCount = 5;

		if(difference > maximumDifference)
			difference = maximumDifference;
		else if(difference < minimumDifference)
			difference = minimumDifference;
		for(pointIndex = 0; pointIndex < pointCount; pointIndex++) {
			if(difference > Element_PowerResistCurve[pointIndex][0])
				break;
		}
		if(pointIndex == pointCount) {
			multiplier = Element_PowerResistCurve[pointIndex - 1][1];
		}
		else {
			differenceSpan = Element_PowerResistCurve[pointIndex - 1][0] -
							 Element_PowerResistCurve[pointIndex][0];
			multiplierSpan = Element_PowerResistCurve[pointIndex - 1][1] -
							 Element_PowerResistCurve[pointIndex][1];
			multiplier =
				(difference - Element_PowerResistCurve[pointIndex][0]) *
					multiplierSpan / differenceSpan +
				Element_PowerResistCurve[pointIndex][1];
		}
		switch(halve) {
		case 0:
			break;
		case 1:
			multiplier /= 2;
			break;
		}
		return multiplier + BATTLE_MULTIPLIER_ONE;
	}
}

/* The battle's random number generator, the same as syRandom with its own */
/* seed, which link battles exchange. */
s32 gmBattleRandom(void) {
	u32 seed;

	seed = ((u32)BattleRandomSeed * 1103515245U) + 12345U;
	btSetRandomSeed(seed);
	return (seed << 8) >> 16;
}

/* Returns a battle random number from 0 to 99. */
u32 gmBattleRandomPercent(void) { return ((u32)gmBattleRandom() * 100) >> 16; }

/* Scales max(attack - defense, 0) plus twice the added power by half the */
/* element multiplier, then halves it. */
s32 gmBattleCalcAttackDamage(s32 attack, s32 defense, s32 addedPower,
							 s32 elementDifference) {
	s32 damage;
	s32 scaledDamage;

	attack -= defense;
	damage = elementDifference;
	if(attack < 0)
		attack = 0;
	scaledDamage =
		gmBattleGetElementMultiplier(damage, 1) * (attack + (addedPower * 2));
	damage = scaledDamage / (2 * BATTLE_MULTIPLIER_ONE);
	if(damage < 0)
		damage = 0;
	return damage;
}

/* Returns Psynergy's power times the element multiplier and powerMultiplier. */
s32 gmBattleCalcPsynergyPower(s32 power, s32 elementDifference,
							  s32 powerMultiplier) {
	s32 scaledPower;

	scaledPower = gmBattleGetElementMultiplier(elementDifference, 0) * power *
				  powerMultiplier;
	return scaledPower / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}

/* Returns the amount healed: power times powerMultiplier and the multiplier */
/* for the healer's element power above HEALING_BASE_POWER. */
s32 gmBattleCalcRecovery(s32 power, s32 elementPower, s32 powerMultiplier) {
	s32 signedProduct;
	u32 scaledRecovery;

	scaledRecovery = gmBattleGetElementMultiplier(
						 (elementPower - HEALING_BASE_POWER) * 2, 0) *
					 power;
	scaledRecovery = powerMultiplier * scaledRecovery;
	signedProduct = scaledRecovery;
	return signedProduct / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}

/* Returns the element the unit attacks with (ELEMENT_NONE for none): an */
/* enemy's comes from its kind, a character's from its equipped weapon. */
s32 gmUnitGetAttackElement(s32 unitId) {
	const ITEM_DEFINITION *item;
	UNIT *unit;

	unit = gmUnitGet(unitId);
	if(unit->classId == CLASS_NONE)
		return gmEnemyGetAttackElement(unit);
	item = gmInventoryGetEquippedDefinition(unit, ITEM_TYPE_WEAPON);
	if(item != NULL)
		return item->element;
	return ELEMENT_NONE;
}

/* Returns the unleash chance, in percent, that a unit's equipment adds */
/* (EQUIP_EFFECT_ADD_UNLEASH_CHANCE). */
s32 gmEquipmentGetUnleashRateBonus(UNIT *unit) {
	s32 chanceBonus;
	s32 inventorySlot;
	s32 effectIndex;
	const ITEM_DEFINITION *item;

	chanceBonus = 0;
	for(inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
		if(unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
			item = gmItemGetDirect(unit->inventory[inventorySlot]);
			for(effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS;
				effectIndex++) {
				if(item->effs[effectIndex].kind ==
				   EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
					chanceBonus += item->effs[effectIndex].amount;
			}
		}
	}
	if(chanceBonus < 0)
		chanceBonus = 0;
	return chanceBonus;
}

/* Rolls whether a character's weapon unleashes, 5 percent a point of its */
/* chance. Returns the unleash's ability, or ABILITY_ATTACK for a plain attack.
 */
s32 gmEquipmentRollUnleash(UNIT *unit) {
	const ITEM_DEFINITION *item;
	FIXED rate;

	if(unit->classId == CLASS_NONE) {
		return ABILITY_ATTACK;
	}
	item = gmInventoryGetEquippedDefinition(unit, ITEM_TYPE_WEAPON);
	if(item == NULL) {
		return ABILITY_ATTACK;
	}
	if(item->unleashAbility == ABILITY_NONE) {
		return ABILITY_ATTACK;
	}
	rate = FIXED_ONE *
		   (gmEquipmentGetUnleashRateBonus(unit) + item->unleashChance * 5) /
		   100;
	if(rate > (gmBattleRandom() & 0xffff)) {
		return item->unleashAbility;
	}
	return ABILITY_ATTACK;
}

/* Returns the base chance in percent of an effect; a negative one is fixed, */
/* and effects not listed always work. */
s32 gmBattleGetEffectBaseSuccessRate(s32 effectId) {
	s32 fixedChance;

	switch(effectId) {
	case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:
	case ABILITY_EFFECT_DEFENSE_DOWN:
		return 70;
	case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY:
	case ABILITY_EFFECT_RESISTANCE_DOWN:
		return 75;
	case ABILITY_EFFECT_CHARM:
		return 30;
	case ABILITY_EFFECT_STUN:
		return 40;
	case ABILITY_EFFECT_SLEEP:
		return 45;
	case ABILITY_EFFECT_POISON:
	case ABILITY_EFFECT_VENOM:
	case ABILITY_EFFECT_SEAL:
		return 55;
	case ABILITY_EFFECT_HAUNT:
		return 25;
	case ABILITY_EFFECT_INSTANT_DEATH:
		return 20;
	case ABILITY_EFFECT_DELUSION:
	case ABILITY_EFFECT_DRAIN_HP:
		return 65;
	case ABILITY_EFFECT_CONFUSION:
	case ABILITY_EFFECT_LEAVE_ONE_HP:
		return 35;
	case ABILITY_EFFECT_HALVE_DEFENSE:
		return 50;
	case ABILITY_EFFECT_REVIVE_HALF:
		fixedChance = 60;
		break;
	case ABILITY_EFFECT_DEW:
		fixedChance = 90;
		break;
	case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY:
	case ABILITY_EFFECT_ATTACK_DOWN:
	case ABILITY_EFFECT_CURSE:
	case ABILITY_EFFECT_DRAIN_PP:
		return 60;
	default:
		fixedChance = 100;
		break;
	}
	return -fixedChance;
}

/* Returns 1 when the unit is weak to the effect: one of the three its enemy */
/* kind or class lists. */
s32 gmBattleAbilityIsWeakToEffect(const UNIT *target, s32 effectId) {
	const u8 *weakEffects;
	s32 weaknessIndex;

	if(target->classId == CLASS_NONE) {
		weakEffects = gmEnemyGetDefinition(target->id)->weakEffs;
		for(weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
			if(*weakEffects == effectId)
				return 1;
			weakEffects++;
		}
	}
	else {
		weakEffects = gmClassGetDefinition(target->classId)->weakEffs;
		for(weaknessIndex = 0; weaknessIndex < 3; weaknessIndex++) {
			if(*weakEffects++ == effectId)
				return 1;
		}
	}
	return 0;
}

/* Returns 1 for the revive effects (Revive, Quartz, Dew), which only work on a
 */
/* downed unit. */
s32 gmBattleIsReviveEffect(s32 effectId) {
	if((effectId == ABILITY_EFFECT_REVIVE) ||
	   (effectId == ABILITY_EFFECT_REVIVE_HALF) ||
	   (effectId == ABILITY_EFFECT_DEW)) {
		return 1;
	}
	return 0;
}

/* The points of chance an effect gains against a target weak to it. */
#define WEAKNESS_BONUS 25

/* Rolls whether an effect takes hold on target: the base rate, plus three */
/* times the caster's lead in `element`'s level, less half the target's luck, */
/* plus WEAKNESS_BONUS against a weakness, times successScale percent. Luff's */
/* seal (ABILITY_EFFECT_SEAL_2_TURNS) gets three tries. */
s32 gmBattleRollEffectSuccess(s32 caster, s32 target, s32 element, s32 effectId,
							  s32 successScale) {
	UNIT *unit = gmUnitGet(target);
	s32 attempts = 1;
	s32 score;
	s32 attempt;
	s8 *poison;
	u8 *delusion;
	u8 *confusion;
	u8 *charm;
	u8 *stun;
	u8 *sleep;

	if(gmBattleIsReviveEffect(effectId) != 0 && unit->hp != 0) {
		return 0;
	}

	if(effectId == ABILITY_EFFECT_CURE_POISON && unit->poison == 0) {
		return 0;
	}

	/* Restore needs a delusion, confusion or charm, or failing those a stun, */
	/* sleep, seal or curse, tested in that order. */
	goto check_restore;
check_restore_others:
	if(unit->stun == 0 && unit->sleep == 0 && *(u8 *)&unit->psySeal == 0 &&
	   unit->grimReaper == 0) {
		return 0;
	}
	goto restore_checked;
check_restore:
	if(effectId == ABILITY_EFFECT_RESTORE) {
		if(unit->delusion == 0 && *(u8 *)&unit->confusion == 0 &&
		   unit->charm == 0) {
			goto check_restore_others;
		}
	}

restore_checked:
	poison = &unit->poison;
	delusion = &unit->delusion;
	confusion = &unit->confusion;
	charm = &unit->charm;
	stun = &unit->stun;
	sleep = &unit->sleep;
	if(effectId == ABILITY_EFFECT_CURE_ALL && *poison == 0 && *delusion == 0 &&
	   *confusion == 0 && *charm == 0 && *stun == 0 && *sleep == 0 &&
	   unit->psySeal == 0 && unit->grimReaper == 0 &&
	   *(u8 *)&unit->haunt == 0) {
		return 0;
	}

	if(effectId == ABILITY_EFFECT_CURSE && unit->grimReaper == 1) {
		return 0;
	}

	/* A positive base rate is adjusted by the element levels, the target's luck
	 */
	/* and its weaknesses; 0 or a negative one is a fixed chance. */
	score = gmBattleGetEffectBaseSuccessRate(effectId);
	if(score > 0) {
		s32 difference = gmUnitGetElementLevel(caster, element) -
						 gmUnitGetElementLevel(target, element) -
						 (unit->luck >> 1);

		score += difference * 3;
		if(gmBattleAbilityIsWeakToEffect(unit, effectId) != 0) {
			score += WEAKNESS_BONUS;
		}
	}
	else {
		score = -score;
	}

	if(effectId == ABILITY_EFFECT_SEAL_2_TURNS) {
		attempts = 3;
	}

	for(attempt = 0; attempt < attempts; attempt++) {
		if(score * successScale / 100 >= (s32)gmBattleRandomPercent()) {
			return 1;
		}
	}
	return 0;
}
