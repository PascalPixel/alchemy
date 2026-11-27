/* An actor's update that flashes it white, two ticks in every four: the Water
   Stone or the Aqua Stone as Douse hits it. */
void scActorFlashWhite(ACTOR *actor)
{
	if(TickCount & 2)
		fdActorSetSprColMode(actor, SPRITE_COLOR_WHITE);
	else
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
}
