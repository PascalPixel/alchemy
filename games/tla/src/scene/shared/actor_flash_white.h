/* An actor's update that flashes it white, two frames in every four: the Water
   Stone or the Aqua Stone as Douse hits it. */
void scActorFlashWhite(ACTOR *actor)
{
	if(FrameCount & 2)
		fdActorSetSprColMode(actor, SPRITE_COLOR_WHITE);
	else
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
}
