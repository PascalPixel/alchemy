/* btEffectRunCuttingEdge, btEffectRunHeatWave and btEffectRunSleet, the same
   in both games. Both games' battle_effect/battle_effect_projectiles.c
   include it where the functions stand. */

/* From the user's leap pose, one large comet in ice colours flies at each
   target and bursts into 80 sparks.

   Abilities: Cutting Edge. */
void btEffectRunCuttingEdge(BATTLE_EFFECT_PARM *args)
{
	btEffectRunVolleyWithAction(args, PROJECTILE_VOLLEY_CUTTING_EDGE);
}

/* From the user's leap pose, seven fireballs trailing embers fly at each
   target, two ticks apart, each bursting into 40 sparks.

   Abilities: Heat Wave. */
void btEffectRunHeatWave(BATTLE_EFFECT_PARM *args)
{
	btEffectRunVolleyWithAction(args, PROJECTILE_VOLLEY_HEAT_WAVE);
}

/* Thirty ice chips fall from high above the user onto each target.

   Abilities: Sleet. */
void btEffectRunSleet(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_SLEET);
}
