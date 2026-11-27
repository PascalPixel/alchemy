/* btEffectRunRecovery, the same in both games. The Broken Seal's
   battle_effect/battle_effect_mirrored_wall.c and The Lost Age's
   battle_effect/battle_effect_rushing_strike.c include it where the function
   stands. */

/* A wide wall of fire rises without shaking the screen.

   Abilities: Recovery. */
void btEffectRunRecovery(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMirroredWall(args, MIRRORED_WALL_RECOVERY);
}
