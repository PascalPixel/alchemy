/* @file */
/* Babi Lighthouse and Venus Lighthouse. */

/* Waits until the actor stops rising or falling, a second at most. */
static void scActorWaitUntilIdle(ACTOR *actor) {
	s32 frames;
	s32 busy;

	frames = FRAMES_PER_SECOND;
	while(frames != 0) {
		sySynch(1);
		busy = actor->velY;
		frames--;
		if(busy == 0)
			break;
	}
}
