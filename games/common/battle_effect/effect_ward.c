/* btEffectRunWard, the same in both games. Both games'
   battle_effect/battle_effect_stat_change.c include it where the function
   stands. */

/* Light swirls into each target and bursts, then a shield appears in Jupiter
   colours.

   Abilities: Ward, Resist, Magic Shell, Magic Shield; Breeze. */
void btEffectRunWard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_WARD);
}
