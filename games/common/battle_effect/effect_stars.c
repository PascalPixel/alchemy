/* btEffectRunSleepStar and btEffectRunSoothingStar, the same in both games.
   Both games' battle_effect/battle_effect_streaks.c include it where the
   functions stand. */

/* Motes burst out of the user and float up, wobbling, as they drift across the
   field onto the targets, and each target flinches in turn.

   Abilities: Sleep Star. */
void btEffectRunSleepStar(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMoteFountain(args, MOTE_FOUNTAIN_DRIFTING);
}

/* Orange healing motes burst out of the user and float up close to it, and each
   target glows in turn with the recovery sound.

   Abilities: Soothing Star. */
void btEffectRunSoothingStar(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMoteFountain(args, MOTE_FOUNTAIN_HEALING);
}
