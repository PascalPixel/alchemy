/* Steps an actor's sprite through the six colour swaps, SPRITE_COLOR_NORMAL to
   SPRITE_COLOR_BRG, one every two ticks: scActorAnimateAndEmit without its
   sparks. Nothing sets it as an update. */
void scActorCycleColorSwaps(ACTOR *actor)
{
	Uint32 frame = TickCount;

	if(frame & 1)
		fdActorSetSpriteColorMode(actor, (frame >> 1) % 6);
}
