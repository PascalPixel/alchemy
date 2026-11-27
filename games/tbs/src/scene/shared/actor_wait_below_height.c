/* @file */
/* Goma Cave Entrance and Goma Cave, and the Lamakan Desert. */

/* Waits until the actor has come down to height (16.16), a second at most. */
static void scActorWaitBelowHeight(ACTOR *actor, FIXED height) {
	s32 frames = 60;

	while(frames != 0) {
		sySynch(1);
		frames--;
		if(actor->y.fixed <= height)
			break;
	}
}
