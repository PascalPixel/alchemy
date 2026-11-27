/* btEffectNoOperation, the same in both games. Both games'
   battle_effect/battle_effect_eruption.c include it where the function
   stands. */

/* Does nothing; btEffectCallNoOperation calls it as a busy wait, in The Broken
   Seal 10,000 times for Annihilation. */
void btEffectNoOperation(void)
{
}
