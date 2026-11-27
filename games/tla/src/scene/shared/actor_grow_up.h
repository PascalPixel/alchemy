/* The first golden ring of a pair (scStaffCreditSpawnGrowingPair): it rises in
   front of the actor it watches, swelling and shrinking over half a sine wave,
   and is released after 32 frames. */
void scActorGrowUp(ACTOR *actor)
{
	ACTOR *anchor = (ACTOR *)actor->watchedActor;
	Sint32 frame = ++actor->tmr;
	FIXED  amplitude;

	if(frame > 31) {
		fdActorRelease(actor);
		return;
	}
	amplitude = syMathSin(frame * (DEGtoANG(180) / 32));
	actor->scaleX = amplitude;
	actor->scaleY = amplitude;

	actor->x.fixed = anchor->x.fixed;
	actor->y.fixed += toFIXED(1);
	actor->z.fixed =
		anchor->z.fixed + (toFIXED(1) - amplitude) * 5 + toFIXED(8);
}
