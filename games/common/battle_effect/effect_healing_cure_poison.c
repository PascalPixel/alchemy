/* btEffectRunCurePoison, btEffectRunUnusedRestoreMotes and btEffectRunTonic,
   the same in both games. Both games' battle_effect/battle_effect_healing.c
   include it where the functions stand. */

/* Sparkles twinkle around each target, four more a tick, and go out one by one
   instead of closing in.

   Abilities: Cure Poison, Restore and, in The Broken Seal, Antidote, Elixir. */
void btEffectRunCurePoison(BATTLE_EFFECT_PARM *args)
{
	if(args->variant == EFFECT_VARIANT_BASIC) {
		btEffectRunHealingMotes(args, HEALING_MODE_CURE_POISON);
		return;
	}
	btEffectRunHealingMotes(args, HEALING_MODE_RESTORE);
}

/* Plays the healing motes as Restore shows them; no effect table entry reaches
   this one. */
UNUSED void btEffectRunUnusedRestoreMotes(BATTLE_EFFECT_PARM *args)
{
	btEffectRunHealingMotes(args, HEALING_MODE_RESTORE);
}

/* Sparkles twinkle around each target, turning on two axes, and go out one by
   one; the runner plays HEALING_MODE_TONIC exactly as HEALING_MODE_RESTORE.

   Abilities: Tonic. */
void btEffectRunTonic(BATTLE_EFFECT_PARM *args)
{
	btEffectRunHealingMotes(args, HEALING_MODE_TONIC);
}
