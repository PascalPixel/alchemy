/* btEffectRunQuickStrike and btEffectRunVorpalSlash, the same in both games.
   Both games' battle_effect/battle_effect_thrash.c include it where the
   functions stand. */

/* The user leaps at the first target, four pictures flash across the effect
   layer, then it fills with one colour as the target is struck.

   Abilities: Quick Strike. */
void btEffectRunQuickStrike(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_QUICK_STRIKE);
}

/* The user steps up to the first target, four pictures flash across the effect
   layer, then it fills with one colour as the target is struck.

   Abilities: Vorpal Slash. */
void btEffectRunVorpalSlash(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_VORPAL_SLASH);
}
