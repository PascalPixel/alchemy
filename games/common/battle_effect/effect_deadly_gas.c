/* btEffectRunDeadlyGas, the same in both games. Both games'
   battle_effect/battle_effect_burst_shower.c include it where the function
   stands. */

/* The user fires a stream of Mars-coloured bursts at the targets, from 28
   pixels in front of it.

   Abilities: Deadly Gas. */
void btEffectRunDeadlyGas(BATTLE_EFFECT_PARM *args)
{
	btEffectRunBurstShower(args, BURST_SHOWER_MODE_DJINNI);
}
