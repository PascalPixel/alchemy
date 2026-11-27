/* gmBattleRandomPercent to gmBattleCalcRecovery, the same in both games.
   Both games' game/game_battle_arithmetic.c include it where the functions
   stand. */

/* Returns a battle random number from 0 to 99. */
Uint32 gmBattleRandomPercent(void)
{
	return ((Uint32)gmBattleRandom() * 100) >> 16;
}

/* Scales max(attack - defense, 0) plus twice the added power by half the
   element multiplier, then halves it. */
Sint32 gmBattleCalcAttackDamage(
	Sint32 attack, Sint32 defense, Sint32 addedPower, Sint32 elementDifference)
{
	Sint32 damage;
	Sint32 scaledDamage;

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
Sint32 gmBattleCalcPsynergyPower(
	Sint32 power, Sint32 elementDifference, Sint32 powerMultiplier)
{
	Sint32 scaledPower;

	scaledPower = gmBattleGetElementMultiplier(elementDifference, 0) * power *
		powerMultiplier;
	return scaledPower / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}

/* Returns the amount healed: power times powerMultiplier and the multiplier for
   the healer's element power above HEALING_BASE_POWER. */
Sint32 gmBattleCalcRecovery(
	Sint32 power, Sint32 elementPower, Sint32 powerMultiplier)
{
	Sint32 signedProduct;
	Uint32 scaledRecovery;

	scaledRecovery = gmBattleGetElementMultiplier(
						 (elementPower - HEALING_BASE_POWER) * 2, 0) *
		power;
	scaledRecovery = powerMultiplier * scaledRecovery;
	signedProduct = scaledRecovery;
	return signedProduct / (BATTLE_MULTIPLIER_ONE * BATTLE_MULTIPLIER_ONE);
}
