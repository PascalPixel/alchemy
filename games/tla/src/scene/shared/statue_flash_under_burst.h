/* The update of a statue Burst is going off at: it blinks red on the ticks its
   mask picks. */
void syStatueFlashUnderBurst(ACTOR *actor)
{
	if(TickCount & actor->flashMask)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
}
