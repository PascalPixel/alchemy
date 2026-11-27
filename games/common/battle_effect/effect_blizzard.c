/* btEffectRunBlizzard, the same in both games. Both games'
   battle_effect/battle_effect_projectiles.c include it where the function
   stands. */

/* From the user's leap pose, sixty ice chips stream at each target, one a
   tick, scattered up and down, without knocking it back.

   Abilities: Blizzard. */
void btEffectRunBlizzard(BATTLE_EFFECT_PARM *args)
{
	btEffectRunVolleyWithAction(args, PROJECTILE_VOLLEY_BLIZZARD);
}
