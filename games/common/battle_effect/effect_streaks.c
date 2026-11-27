/* btEffectRunSonicWave to btEffectRunLimeStreaks, the same in both games.
   Both games' battle_effect/battle_effect_streaks.c include it where the
   functions stand. */

/* Three streaks of ember light fly from the user at each target in turn; each
   target flashes and recoils.

   Abilities: Sonic Wave. */
void btEffectRunSonicWave(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_EMBER);
}

/* Three streaks of lime light fly from the user at each target in turn; each
   target flashes and recoils.

   Abilities: Shriek and, in The Lost Age, Harpy. */
void btEffectRunShriek(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_LIME);
}

/* Three streaks of violet light fly from the user at each target in turn; each
   target flashes and trembles.

   Abilities: Banshee Howl. */
void btEffectRunBansheeHowl(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_VIOLET);
}

/* Three flickering cyan streaks, cycling through three colour rows, fly from
   the user at each target in turn; each target flashes and recoils.

   Abilities: War Cry. */
void btEffectRunWarCry(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_CYAN);
}

/* Three flickering red streaks, cycling through three colour rows, fly from the
   user at each target in turn; each target flashes and trembles.

   Abilities: Wicked Howl. */
void btEffectRunWickedHowl(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_RED);
}

/* Unused: no entry of the effect table points here. It would have played the
   same lime streaks as Shriek. */
UNUSED void btEffectRunLimeStreaks(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStreakVolley(args, STREAK_STYLE_LIME);
}
