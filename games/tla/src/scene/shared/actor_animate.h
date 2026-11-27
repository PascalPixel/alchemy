/* Steps an actor's sprite through the six colour swaps, SPRITE_COLOR_NORMAL to
   SPRITE_COLOR_BRG, one every two ticks: scActorAnimateAndEmit without its
   sparks. Nothing sets it as an update. */
void scActorCycleColSwaps(ACTOR *actor)
{
	Uint32 frm = TickCount;

	if(frm & 1)
		fdActorSetSprColMode(actor, (frm >> 1) % 6);
}
