/* btAbilityIsPsynergy, the same in both games. The Broken Seal's
   battle/battle_ability.c and The Lost Age's battle/battle_targeting.c
   include it where the function stands. */

/* Returns 1 for an ability that costs PP, and for Psy Drain whatever its
   cost. */
Uint32 btAbilityIsPsynergy(Sint32 abilityId)
{
	Uint8 pp;

	if(abilityId == ABILITY_PSY_DRAIN)
		return 1;
	pp = gmAbilityGetDirectStub(abilityId)->ppCost;
	return pp != 0;
}
