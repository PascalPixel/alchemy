/* btEffectRunImpact and btEffectRunGuard, the same in both games. Both
   games' battle_effect/battle_effect_stat_change.c include it where the
   functions stand. */

/* Light swirls into each target and bursts, then a dagger appears in Mars
   colours.

   Abilities: Impact, High Impact, Demon Spear, Angel Spear; Forge; in The Lost
   Age, Kindle. */
void btEffectRunImpact(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_IMPACT);
}

/* Light swirls into each target and bursts, then a shield appears in Mars
   colours.

   Abilities: Guard, Protect, Guardian, Protector; Corona. */
void btEffectRunGuard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatBoost(args, STAT_BOOST_GUARD);
}
