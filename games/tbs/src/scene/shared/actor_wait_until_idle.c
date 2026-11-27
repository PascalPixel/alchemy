/* Babi Lighthouse and Venus Lighthouse. */

/* Waits until the actor stops rising or falling, a second at most. */
static void scActorWaitUntilIdle(ACTOR *actor)
{
	Sint32 frames;
	Sint32 busy;

	frames = FRAMES_PER_SECOND;
	while(frames != 0) {
		sySynch(1);
		busy = actor->velY;
		frames--;
		if(busy == 0)
			break;
	}
}
