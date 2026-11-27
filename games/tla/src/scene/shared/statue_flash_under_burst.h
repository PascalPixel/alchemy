/* The update of a statue Burst is going off at: it blinks red on the frames */
/* its mask picks. */
void syStatueFlashUnderBurst(ACTOR *actor) {
	if(FrameCount & actor->flashMask)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
}
