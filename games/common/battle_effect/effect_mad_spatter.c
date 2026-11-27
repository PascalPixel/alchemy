/* btEffectRunMadSpatter, the same in both games. The Broken Seal's
   battle_effect/battle_effect_lobbed_shells.c and The Lost Age's
   battle_effect/battle_effect_djinn_beams.c include it where the function
   stands. */

/* Gold shells arc from the user onto each target in turn and burst into shards.

   Abilities: Mad Spatter. */
void btEffectRunMadSpatter(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_MAD_SPATTER);
}
