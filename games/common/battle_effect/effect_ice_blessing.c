/* btEffectRunIceBlessing, the same in both games. Both games'
   battle_effect/battle_effect_projectiles.c include it where the function
   stands. */

/* Sixty ice chips stream at each target, one a tick, without knocking it back.

   Abilities: Ice Blessing in The Broken Seal; Ice Breath, Blue Dragon in The
   Lost Age. */
void btEffectRunIceBlessing(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_ICE_BLESSING);
}
