/* @file */
/* Waiting for an actor to stop rising or falling. */

#include "system.h"
#include "field/actor.h"

/* Waits until the actor stops rising or falling, sixty frames at most. */
void fdActorWaitUntilLanded(ACTOR *actor) {
	s32 remainingFrames = 60;
	s32 busy;

	while(remainingFrames != 0) {
		sySynch(1);
		busy = actor->velY;
		remainingFrames--;
		if(busy == 0)
			break;
	}
}
