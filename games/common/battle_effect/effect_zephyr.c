/* btEffectRunZephyr, the same in both games. Both games'
   battle_effect/battle_effect_stat_change.c include it where the function
   stands. */

/* Light swirls into each target and bursts, then a gust of wind sweeps across
   it in the colours already loaded, as this is the one boost that keeps the
   palette.

   Abilities: Zephyr. */
void btEffectRunZephyr(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_ZEPHYR);
}
