/* fdEffectAdvanceSpinAng, the same in both games. Both games'
   field/script_commands_djinni.c include it where the function stands. */

/* Per-tick update: spins an actor 1/8 turn a tick. */
void fdEffectAdvanceSpinAng(ACTOR *actor)
{
	actor->facing += DEGtoANG(45);
}
