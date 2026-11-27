/* btEffectRunDecompose to btEffectRunRottenBlood, the same in both games.
   The Broken Seal's battle_effect/battle_effect_lobbed_shells.c and The Lost
   Age's battle_effect/battle_effect_djinn_beams.c include it where the
   functions stand. */

/* Green shells arc from the user onto each target in turn and burst into smoke;
   in The Lost Age, Dull's effect follows.

   Abilities: Decompose. */
void btEffectRunDecompose(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_DECOMPOSE);
}

/* Blue shells arc from the user onto each target in turn and burst into smoke.

   Abilities: Bone Chiller. */
void btEffectRunBoneChiller(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_BONE_CHILLER);
}

/* Shells in the spider web's colours arc from the user onto each target in turn
   and burst into smoke.

   Abilities: Sticky Goo. */
void btEffectRunStickyGoo(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_STICKY_GOO);
}

/* Shells in Mars Djinn colours arc from the user onto each target in turn and
   burst into smoke.

   Abilities: Poison Ink. */
void btEffectRunPoisonInk(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_POISON_INK);
}

/* Lime shells arc from the user onto each target in turn and burst into smoke.

   Abilities: Sticky Poison. */
void btEffectRunStickyPoison(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_STICKY_POISON);
}

/* Green shells arc from the user onto each target in turn and burst into
   shards; in The Lost Age, Impair's effect follows.

   Abilities: Rotten Blood. */
void btEffectRunRottenBlood(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_ROTTEN_BLOOD);
}
