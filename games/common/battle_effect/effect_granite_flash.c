/* btEffectRunGranite and btEffectRunFlash, the same in both games. Both
   games' battle_effect/battle_effect_stat_change.c include it where the
   functions stand. */

/* Light swirls into each target and bursts, then a pane of light appears in
   Venus colours.

   Abilities: Granite. */
void btEffectRunGranite(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_GRANITE);
}

/* Light swirls into each target and bursts, then a pane of light appears in
   Mars colours.

   Abilities: Flash. */
void btEffectRunFlash(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_FLASH);
}
