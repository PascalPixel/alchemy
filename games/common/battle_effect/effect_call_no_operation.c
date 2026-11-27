/* btEffectCallNoOperation, the same in both games. Both games'
   battle_effect/battle_effect_dispatch.c include it where the function
   stands. */

/* Calls the empty btEffectNoOperation count times: a busy delay. */
void btEffectCallNoOperation(Sint32 cnt)
{
	Sint32 i;

	for(i = 0; i != cnt; i++)
		btEffectNoOperation();
}
