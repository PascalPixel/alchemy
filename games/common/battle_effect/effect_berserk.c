/* btEffectRunBerserk, the same in both games. The Broken Seal's
   battle_effect/battle_effect_mirrored_wall.c and The Lost Age's
   battle_effect/battle_effect_rushing_strike.c include it where the function
   stands. */

/* The wall rises wide and shakes the screen hard.

   Abilities: Berserk. */
void btEffectRunBerserk(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMirroredWall(args, MIRRORED_WALL_BERSERK);
}
