/* btEffectRunDelude to btEffectRunDrown, the same in both games. Both games'
   battle_effect/battle_effect_target_motes.c include it where the functions
   stand. */

/* Cyan smoke rises around each target.

   Abilities: Delude, Confuse, Mist and, in The Lost Age, Mucous Gel. */
void btEffectRunDelude(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_DELUDE);
}

/* Smoke rises around each target, and glints sparkle near it.

   Abilities: Paralyze. */
void btEffectRunParalyze(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_PARALYZE);
}

/* Hearts close in on each target, turning as they come.

   Abilities: Heartrender. */
void btEffectRunHeartrender(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_HEARTRENDER);
}

/* Glowing orbs spiral in on each target.

   Abilities: Mystic Flame. */
void btEffectRunMysticFlame(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_MYSTIC_FLAME);
}

/* Glowing orbs in Mars Djinn colours rise around each target, and then the user
   steps up to the first target and strikes it.

   Abilities: Demon Fire, which The Lost Age spells Demonfire. */
void btEffectRunDemonFire(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_DEMON_FIRE);
}

/* Thirty-two water drops spiral in on each target.

   Abilities: Drown. */
void btEffectRunDrown(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetMotes(args, TARGET_MOTES_DROWN);
}
