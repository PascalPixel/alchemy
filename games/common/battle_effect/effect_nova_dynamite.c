/* btEffectRunNova and btEffectRunDynamite, the same in both games. Both
   games' battle_effect/battle_effect_plasma.c include it where the functions
   stand. */

/* Bursts of light go off among the targets, behind the battle's first layer,
   each ringed by fading flashes and throwing sparks.

   Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb. */
void btEffectRunNova(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSparkGroups(args, SPARK_GROUPS_NOVA);
}

/* Orange bursts go off in front of the battle, which darkens behind them.

   Abilities: Dynamite and, in The Lost Age, Angry Mine. */
void btEffectRunDynamite(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSparkGroups(args, SPARK_GROUPS_DYNAMITE);
}
