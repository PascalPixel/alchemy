/* Kalay and Venus Lighthouse's aerie: people who look about. */

/* An actor script call, each tick: when the actor's countdown (userValue) runs
   out, it turns by a random amount up to half a turn and waits a random 0 to 79
   ticks. */
static Sint32 scLookAboutRandomly(ACTOR *actor)
{
	Uint32 nextTmr;

	if(actor->userValue == 0) {
		actor->facing += syRandomBelow(32768);
		nextTmr = syRandomBelow(80);
		actor->userValue = nextTmr;
	}
	if(actor->userValue != 0) {
		actor->userValue--;
	}
	return 1;
}
