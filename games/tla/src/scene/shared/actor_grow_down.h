/* The second golden ring of a pair (scStaffCreditSpawnGrowingPair): drawn
   upside down, it rises behind the actor it watches, swelling and shrinking
   over half a sine wave, and is released after 32 ticks. */
void scActorGrowDown(ACTOR *actor)
{
	ACTOR *anchor = (ACTOR *)actor->watchedActor;
	Sint32 frm = ++actor->tmr;
	FIXED  amplitude;

	if(frm > 31) {
		fdActorRelease(actor);
		return;
	}
	amplitude = syMathSin(frm * (DEGtoANG(180) / 32));
	actor->scaleX = amplitude;
	actor->scaleY = -amplitude;

	actor->x.fixed = anchor->x.fixed;
	actor->y.fixed += toFIXED(1);
	actor->z.fixed =
		anchor->z.fixed - (toFIXED(1) - amplitude) * 5 + toFIXED(16);
}
