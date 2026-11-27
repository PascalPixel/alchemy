/* btEffectRunBearClaw to btEffectRunFreebiteRush, the same in both games.
   Both games' battle_effect/battle_effect_flurry.c include it where the
   functions stand. */

/* The user strikes the first target five times.

   Abilities: Bear Claw; Counterstrike. */
void btEffectRunBearClaw(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_BEAR_CLAW);
}

/* Ransack plays the same five strikes as Bear Claw.

   Abilities: Ransack; Mad Dash. */
void btEffectRunRansack(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_BEAR_CLAW);
}

/* Seven strikes land on the first target in the lime palette.

   Abilities: Bacteria Rush. */
void btEffectRunBacteriaRush(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_BACTERIA_RUSH);
}

/* Nine strikes land on the first target in the blue palette.

   Abilities: Freebite Rush. */
void btEffectRunFreebiteRush(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlurry(args, FLURRY_FREEBITE_RUSH);
}
