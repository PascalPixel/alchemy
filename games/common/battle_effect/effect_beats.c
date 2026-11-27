/* btEffectRunPoisonBeat and btEffectRunSpinningBeat, the same in both games.
   Both games' battle_effect/battle_effect_spider_web.c include it where the
   functions stand. */

/* A lime swirl sweeps across the first target and knocks it back.

   Abilities: Poison Beat. */
void btEffectRunPoisonBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSwirlSwipe(args, SWIRL_SWIPE_POISON_BEAT);
}

/* A magenta swirl sweeps across the first target and throws it into the air.

   Abilities: Spinning Beat. */
void btEffectRunSpinningBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSwirlSwipe(args, SWIRL_SWIPE_SPINNING_BEAT);
}
