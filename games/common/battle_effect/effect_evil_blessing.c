/* btEffectRunEvilBlessing, the same in both games. Both games'
   battle_effect/battle_effect_burst_shower.c include it where the function
   stands. */

/* The user fires a stream of demon faces at the targets, from 16 pixels in
   front of it.

   Abilities: Evil Blessing. */
void btEffectRunEvilBlessing(BATTLE_EFFECT_PARM *args)
{
	btEffectRunBurstShower(args, BURST_SHOWER_MODE_FACES);
}
