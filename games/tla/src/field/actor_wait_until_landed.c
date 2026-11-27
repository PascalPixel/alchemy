/* Waiting for an actor to stop rising or falling. */

#include "system.h"
#include "field/actor.h"

/* Waits until the actor stops rising or falling, sixty ticks at most. */
void fdActorWaitUntilLanded(ACTOR *actor)
{
	Sint32 remainingTicks = 60;
	Sint32 busy;

	while(remainingTicks != 0) {
		sySynch(1);
		busy = actor->velY;
		remainingTicks--;
		if(busy == 0)
			break;
	}
}
