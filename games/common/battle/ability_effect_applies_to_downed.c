/* btAbilityEffectAppliesToDowned, the same in both games. The Broken Seal's
   battle/battle_ability.c and The Lost Age's battle/battle_targeting.c
   include it where the function stands. */

/* Returns 1 for an effect that still applies when the ability's damage has just
   knocked its target out: the drains, which give the damage they dealt back to
   the user. */
Bool btAbilityEffectAppliesToDowned(Sint32 effect)
{
	switch(effect) {
	case ABILITY_EFFECT_DRAIN_HP:
	case ABILITY_EFFECT_DRAIN_PP:
	case ABILITY_EFFECT_DRAIN_HALF_HP:
	case ABILITY_EFFECT_PSY_LEECH:
		return TRUE;
	default:
		return FALSE;
	}
}
