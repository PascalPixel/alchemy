// Battle arithmetic: element multipliers, random rolls and success rates. Part of game.c.

// Returns the multiplier, 256 for 1x, for an element difference: 256 plus 1.28 a point, the
// difference held to -200 to 200, its effect half with `halve`.
Sint32 gmBattleGetElementMultiplier(Sint32 difference, Sint32 halve)
{
	Sint32 minimumDifference;
	Sint32 maximumDifference;
	Sint32 pointCount;
	Sint32 pointIndex;
	Sint32 multiplier;
	Sint32 differenceSpan;
	Sint32 multiplierSpan;

	maximumDifference = ElementPowerResistCurve[0][0];
	pointCount        = ARRAY_COUNT(ElementPowerResistCurve);
	minimumDifference = ElementPowerResistCurve[pointCount - 1][0];
	if (difference > maximumDifference)
		difference = maximumDifference;
	else if (difference < minimumDifference)
		difference = minimumDifference;
	for (pointIndex = 0; pointIndex < pointCount; pointIndex++)
	{
		if (difference > ElementPowerResistCurve[pointIndex][0])
			break;
	}
	if (pointIndex == pointCount)
	{
		multiplier = ElementPowerResistCurve[pointIndex - 1][1];
	}
	else
	{
		differenceSpan =
			ElementPowerResistCurve[pointIndex - 1][0] - ElementPowerResistCurve[pointIndex][0];
		multiplierSpan =
			ElementPowerResistCurve[pointIndex - 1][1] - ElementPowerResistCurve[pointIndex][1];
		multiplier =
			(difference - ElementPowerResistCurve[pointIndex][0]) * multiplierSpan / differenceSpan
			+ ElementPowerResistCurve[pointIndex][1];
	}
	switch (halve)
	{
		case 0: break;
		case 1: multiplier /= 2; break;
	}
	return multiplier + BATTLE_MULTIPLIER_ONE;
}

// The battle's random number generator, the same as syRandom with its own seed: returns bits 8 to
// 23 of the new seed.
Uint32 gmBattleRandom(void)
{
	Sint32 seed;

	seed             = (BattleRandomSeed * 1103515245) + 12345;
	BattleRandomSeed = seed;
	return (Uint32)(seed << 8) >> 16;
}

// Returns a battle random number from 0 to 99.
Uint32 gmBattleRandomPercent(void)
{
	return ((Uint32)gmBattleRandom() * 100) >> 16;
}

// Scales max(attack - defense, 0) plus twice the added power by half the element multiplier, then
// halves it.
Sint32 gmBattleCalculateAttackDamage(Sint32 attack,
                                     Sint32 defense,
                                     Sint32 addedPower,
                                     Sint32 elementDifference)
{
	Sint32 damage;
	Sint32 scaledDamage;

	attack -= defense;
	damage  = elementDifference;
	if (attack < 0)
		attack = 0;
	scaledDamage = gmBattleGetElementMultiplier(damage, 1) * (attack + (addedPower * 2));
	damage       = scaledDamage / (2 * BATTLE_MULTIPLIER_ONE);
	if (damage < 0)
		damage = 0;
	return damage;
}

// Returns Psynergy's power times the element multiplier and powerMultiplier.
Sint32
gmBattleCalculatePsynergyPower(Sint32 power, Sint32 elementDifference, Sint32 powerMultiplier)
{
	Sint32 scaledPower;

	scaledPower = gmBattleGetElementMultiplier(elementDifference, 0) * power * powerMultiplier;
	return scaledPower / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}

// Returns the amount healed: power times powerMultiplier and the multiplier for the healer's
// element power above HEALING_BASE_POWER.
Sint32 gmBattleCalculateRecovery(Sint32 power, Sint32 elementPower, Sint32 powerMultiplier)
{
	Sint32 signedProduct;
	Uint32 scaledRecovery;

	scaledRecovery =
		gmBattleGetElementMultiplier((elementPower - HEALING_BASE_POWER) * 2, 0) * power;
	scaledRecovery = powerMultiplier * scaledRecovery;
	signedProduct  = scaledRecovery;
	return signedProduct / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}

// Returns the element the unit attacks with (ELEMENT_NONE for none): an enemy's comes from its
// kind, a character's from its equipped weapon.
Sint32 gmUnitGetAttackElement(Sint32 unitId)
{
	const ITEM_DEFINITION *item;
	UNIT                  *unit;

	unit = syUnitGet(unitId);
	if (unit->classId == CLASS_NONE)
		return gmEnemyGetAttackElement(unit);
	item = gmInventoryGetEquippedDefinition(unit, ITEM_TYPE_WEAPON);
	if (item != NULL)
		return item->element;
	return ELEMENT_NONE;
}

// Returns the unleash chance, in percent, that a unit's equipment adds
// (EQUIP_EFFECT_ADD_UNLEASH_CHANCE).
Sint32 gmEquipmentGetUnleashRateBonus(UNIT *unit)
{
	Sint32                 chanceBonus;
	Sint32                 inventorySlot;
	Sint32                 effectIndex;
	const ITEM_DEFINITION *item;

	chanceBonus = 0;
	for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++)
	{
		if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED)
		{
			item = gmItemGetDirect(unit->inventory[inventorySlot]);
			for (effectIndex = 0; effectIndex < EQUIP_EFFECT_SLOTS; effectIndex++)
			{
				if (item->effects[effectIndex].kind == EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
					chanceBonus += item->effects[effectIndex].amount;
			}
		}
	}
	if (chanceBonus < 0)
		chanceBonus = 0;
	return chanceBonus;
}

// Rolls whether a character's weapon unleashes, 5 percent a point of its chance. Returns the
// unleash's ability, or ABILITY_ATTACK for a plain attack.
Sint32 gmEquipmentRollUnleash(UNIT *owner)
{
	ITEM_DEFINITION *item;
	FIXED            rate;

	if (owner->classId == CLASS_NONE)
	{
		return ABILITY_ATTACK;
	}
	item = gmInventoryGetEquippedDefinition(owner, ITEM_TYPE_WEAPON);
	if (item == NULL)
	{
		return ABILITY_ATTACK;
	}
	if (item->unleashAbility == ABILITY_NOTHING)
	{
		return ABILITY_ATTACK;
	}
	rate = toFIXED(1) * (gmEquipmentGetUnleashRateBonus(owner) + item->unleashChance * 5) / 100;
	if (rate > (Sint32)(gmBattleRandom() & 0xFFFF))
	{
		return item->unleashAbility;
	}
	return ABILITY_ATTACK;
}

// Returns the base chance in percent of an effect; a negative one is fixed, and effects not listed
// always work.
Sint32 gmBattleGetEffectBaseSuccessRate(Sint32 effectId)
{
	Sint32 fixedChance;

	switch (effectId)
	{
		case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:
		case ABILITY_EFFECT_DEFENSE_DOWN           : return 70;
		case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY:
		case ABILITY_EFFECT_RESISTANCE_DOWN        : return 75;
		case ABILITY_EFFECT_CHARM                  : return 30;
		case ABILITY_EFFECT_SLEEP                  : return 45;
		case ABILITY_EFFECT_POISON                 :
		case ABILITY_EFFECT_VENOM                  :
		case ABILITY_EFFECT_SEAL                   : return 55;
		case ABILITY_EFFECT_HAUNT                  : return 25;
		case ABILITY_EFFECT_INSTANT_DEATH          : return 20;
		case ABILITY_EFFECT_DELUSION               :
		case ABILITY_EFFECT_DRAIN_HP               : return 65;
		case ABILITY_EFFECT_CONFUSION              :
		case ABILITY_EFFECT_LEAVE_ONE_HP           : return 35;
		case ABILITY_EFFECT_REVIVE_HALF            : fixedChance = 60; break;
		case ABILITY_EFFECT_DEW                    : fixedChance = 90; break;
		case ABILITY_EFFECT_BALM                   : fixedChance = 70; break;
		case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY    :
		case ABILITY_EFFECT_ATTACK_DOWN            :
		case ABILITY_EFFECT_CURSE                  :
		case ABILITY_EFFECT_DRAIN_PP               :
		case ABILITY_EFFECT_CURSE_THREE_ROLLS      : return 60;
		case ABILITY_EFFECT_HALVE_DEFENSE          :
		case ABILITY_EFFECT_ULYSSES                : return 50;
		case ABILITY_EFFECT_STUN                   :
		case ABILITY_EFFECT_BLOW_AWAY              :
		case ABILITY_EFFECT_STUN_THREE_ROLLS       : return 40;
		default                                    : fixedChance = 100; break;
	}
	return -fixedChance;
}

// Returns 1 when enemy `enemyId` is one of EnemyDragonIds.
Bool gmEnemyIsDragon(Sint32 enemyId)
{
	Uint32 i;

	for (i = 0; i < DRAGON_ENEMY_COUNT; i++)
	{
		if (enemyId == EnemyDragonIds[i])
			return TRUE;
	}
	return FALSE;
}

// Returns 1 when enemy `enemyId` is one of EnemySeaCreatureIds.
Bool gmEnemyIsSeaCreature(Sint32 enemyId)
{
	Uint32 i;

	for (i = 0; i < SEA_CREATURE_ENEMY_COUNT; i++)
	{
		if (enemyId == EnemySeaCreatureIds[i])
			return TRUE;
	}
	return FALSE;
}

// Returns 1 when the unit is weak to effect `effectId`: one of three listed for its enemy kind
// (ENEMY_DEFINITION.weakEffects) or class (CLASS_DEFINITION.weakEffects). Called by
// gmBattleRollEffectSuccess.
Bool gmBattleAbilityIsWeakToEffect(const UNIT *target, Sint32 effectId)
{
	const Uint8 *entries;
	Sint32       i;

	if (target->classId == CLASS_NONE)
	{
		entries = gmEnemyGetDefinition(target->enemyId)->weakEffects;
		for (i = 0; i < 3; i++)
		{
			if (*entries == effectId)
				return TRUE;
			entries++;
		}
	}
	else
	{
		entries = gmClassGetDefinition(target->classId)->weakEffects;
		for (i = 0; i < 3; i++)
		{
			if (*entries++ == effectId)
				return TRUE;
		}
	}
	return FALSE;
}

// Returns 1 for the revive effects, which only work on a downed unit.
Bool gmBattleIsReviveEffect(Sint32 effectId)
{
	if (effectId == ABILITY_EFFECT_REVIVE || effectId == ABILITY_EFFECT_REVIVE_HALF
	    || effectId == ABILITY_EFFECT_DEW || effectId == ABILITY_EFFECT_BALM)
	{
		return TRUE;
	}
	return FALSE;
}

// The Luck at which a target shrugs off poison, haunting, instant death, the curse, Leave One HP,
// Gasp and Gale.
#define LUCK_IMMUNITY  40
// The points of chance an effect gains against a target weak to it.
#define WEAKNESS_BONUS 25

// Rolls whether effect `effectId` takes hold on `target`: it fails at once when there is nothing
// for it to do. The chance is the base rate, plus three times the caster's lead over the target in
// `element`'s level less half the target's luck (so every two points of luck take off three), plus
// WEAKNESS_BONUS against a weakness, times `successScale` percent. A base rate of 0 or less counts
// as its magnitude, still scaled but skipping the levels, luck and weakness; 0 still succeeds when
// gmBattleRandomPercent gives 0, about 1 time in 100. Luff, Gasp and Fire Puppet's Paralyze get
// three tries. Raiden's Wrath and the Trident take hold only on a dragon or a sea creature; a
// target with LUCK_IMMUNITY Luck or more shrugs off poison, haunting, instant death, the curse,
// Leave One HP, Gasp and Gale, and a character Gale too.
Sint32 gmBattleRollEffectSuccess(
	Sint32 caster, Sint32 target, Sint32 element, Sint32 effectId, Sint32 successScale)
{
	UNIT  *unit     = syUnitGet(target);
	Sint32 attempts = 1;
	Sint32 score;
	Sint32 attempt;
	Uint8 *luck;
	Sint8 *poison;
	Uint8 *delusion;
	Uint8 *confusion;
	Uint8 *charm;
	Uint8 *stun;
	Uint8 *sleep;

	// Raiden's Wrath works only on dragons, the Trident only on sea creatures.
	if (effectId == ABILITY_EFFECT_SLAY_DRAGON)
		return gmEnemyIsDragon(unit->enemyId);
	if (effectId == ABILITY_EFFECT_SLAY_SEA_CREATURE)
		return gmEnemyIsSeaCreature(unit->enemyId);

	if (gmBattleIsReviveEffect(effectId) != 0 && unit->hp != 0)
	{
		return 0;
	}

	if (effectId == ABILITY_EFFECT_CURE_POISON && unit->poison == 0)
	{
		return 0;
	}

	// Restore needs a delusion, confusion or charm, or failing those a stun, sleep, seal or curse,
	// tested in that order.
	goto check_restore;
check_restore_others:
	if (unit->stun == 0 && unit->sleep == 0 && *(Uint8 *)&unit->psySeal == 0
	    && unit->grimReaper == 0)
	{
		return 0;
	}
	goto restore_checked;
check_restore:
	if (effectId == ABILITY_EFFECT_RESTORE)
	{
		if (unit->delusion == 0 && *(Uint8 *)&unit->confusion == 0 && unit->charm == 0)
		{
			goto check_restore_others;
		}
	}

restore_checked:
	poison    = &unit->poison;
	delusion  = &unit->delusion;
	confusion = (Uint8 *)&unit->confusion;
	charm     = &unit->charm;
	stun      = &unit->stun;
	sleep     = &unit->sleep;
	if (effectId == ABILITY_EFFECT_CURE_ALL && *poison == 0 && *delusion == 0 && *confusion == 0
	    && *charm == 0 && *stun == 0 && *sleep == 0 && unit->psySeal == 0 && unit->grimReaper == 0
	    && *(Uint8 *)&unit->haunt == 0)
	{
		return 0;
	}

	if (effectId == ABILITY_EFFECT_CURSE && unit->grimReaper == 1)
	{
		return 0;
	}

	luck = &unit->luck;
	if (*luck >= LUCK_IMMUNITY)
	{
		switch (effectId)
		{
			case ABILITY_EFFECT_POISON:
			case ABILITY_EFFECT_VENOM:
			case ABILITY_EFFECT_HAUNT:
			case ABILITY_EFFECT_INSTANT_DEATH:
			case ABILITY_EFFECT_CURSE:
			case ABILITY_EFFECT_LEAVE_ONE_HP:
			case ABILITY_EFFECT_CURSE_THREE_ROLLS:
			case ABILITY_EFFECT_BLOW_AWAY        : return 0;
		}
	}
	if (unit->classId != CLASS_NONE && effectId == ABILITY_EFFECT_BLOW_AWAY)
		return 0;

	// A positive base rate is adjusted by the element levels, the target's luck and its weaknesses;
	// 0 or a negative one is a fixed chance.
	score = gmBattleGetEffectBaseSuccessRate(effectId);
	if (score > 0)
	{
		Sint32 difference = gmUnitGetElementLevel(caster, element)
		                  - gmUnitGetElementLevel(target, element) - (*luck >> 1);
		score += difference * 3;
		if (gmBattleAbilityIsWeakToEffect(unit, effectId) != 0)
		{
			score += WEAKNESS_BONUS;
		}
	}
	else
	{
		score = -score;
	}

	if (effectId == ABILITY_EFFECT_SEAL_2_TURNS)
		attempts = 3;
	if (effectId == ABILITY_EFFECT_CURSE_THREE_ROLLS)
		attempts = 3;
	if (effectId == ABILITY_EFFECT_STUN_THREE_ROLLS)
		attempts = 3;

	for (attempt = 0; attempt < attempts; attempt++)
	{
		if (score * successScale / 100 >= (Sint32)gmBattleRandomPercent())
		{
			return 1;
		}
	}
	return 0;
}
