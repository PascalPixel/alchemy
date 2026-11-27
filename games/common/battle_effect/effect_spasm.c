/* btEffectRunSpasm, the same in both games. The Broken Seal's
   battle_effect/battle_effect_mirrored_wall.c and The Lost Age's
   battle_effect/battle_effect_rushing_strike.c include it where the function
   stands. */

/* The wall rises with a long gentle shake and the recovery sound.

   Abilities: Spasm. */
void btEffectRunSpasm(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMirroredWall(args, MIRRORED_WALL_SPASM);
}
