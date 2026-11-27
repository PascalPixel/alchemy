/* btEffectRunTaint, the same in both games. Both games'
   battle_effect/battle_effect_target_motes.c include it where the function
   stands. */

/* Lime smoke swirls in on each target, or pink smoke for a stronger variant.

   Abilities: Taint, Poison. */
void btEffectRunTaint(BATTLE_EFFECT_PARM *args)
{
	if(args->variant == EFFECT_VARIANT_BASIC) {
		btEffectRunTargetMotes(args, TARGET_MOTES_TAINT);
		return;
	}
	btEffectRunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}
