/* Steps an actor's sprite through the six colour swaps, SPRITE_COLOR_NORMAL to
   SPRITE_COLOR_BRG, one every two frames: scActorAnimateAndEmit without its
   sparks. Nothing sets it as an update. */
void scActorCycleColSwaps(ACTOR *actor)
{
	Uint32 frame = FrameCount;

	if(frame & 1)
		fdActorSetSprColMode(actor, (frame >> 1) % 6);
}
