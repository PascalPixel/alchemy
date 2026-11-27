/* btEffectRunDull, btEffectRunImpair and btEffectRunWeaken, the same in both
   games. Both games' battle_effect/battle_effect_stat_change.c include it
   where the functions stand. */

/* A dagger appears over each target and breaks into shards that fade through
   darker copies, in the pink burst's colours in The Broken Seal and the glow
   orb's in The Lost Age.

   Abilities: Dull, Blunt. */
void btEffectRunDull(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatReduction(args, STAT_REDUCTION_DULL);
}

/* A shield appears over each target and breaks into shards that fade through
   darker copies, in the glow orb's colours.

   Abilities: Impair, Debilitate. */
void btEffectRunImpair(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatReduction(args, STAT_REDUCTION_IMPAIR);
}

/* A shield appears over each target and breaks into fading shards, in the pink
   burst's colours.

   Abilities: Weaken, Enfeeble. */
void btEffectRunWeaken(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStatReduction(args, STAT_REDUCTION_WEAKEN);
}
