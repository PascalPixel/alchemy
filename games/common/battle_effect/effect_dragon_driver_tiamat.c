/* btEffectRunDragonDriver and btEffectRunTiamat, the same in both games. The
   Broken Seal's battle_effect/battle_effect_tiamat.c and The Lost Age's
   battle_effect/battle_effect_procne.c include it where the functions stand. */

/* Fireballs rain on the targets and a blast of fire sweeps across them, in the
   LightningBolt colours.

   Abilities: Dragon Driver, in The Broken Seal the Fusion Dragon's. */
void btEffectRunDragonDriver(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDragonFire(args, DRAGON_FIRE_DRAGON_DRIVER);
}

/* Tiamat lands while fireballs rain on the targets, breathes fire across them,
   and bursts away.

   Abilities: Tiamat. */
void btEffectRunTiamat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDragonFire(args, DRAGON_FIRE_TIAMAT);
}
