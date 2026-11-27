/* btEffectRunThrash, the same in both games. Both games'
   battle_effect/battle_effect_thrash.c include it where the function stands. */

/* The user leaps high at the first target, then sparks burst beneath it.

   Abilities: Thrash. */
void btEffectRunThrash(BATTLE_EFFECT_PARM *args)
{
	btMotionApproachTargetStub(args->user, args->tgts[0], 24, toFIXED(61) / 5);
	sySynch(29);
	args->variant = SPARK_GROUPS_THRASH;
	btEffectRunSparkGroups(args, SPARK_GROUPS_RISING);
}
