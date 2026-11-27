/* Each tick, a puff of chimney smoke drifts sideways ever faster, rises and
   spreads, until its ticks run out and it is gone. */
void scSmokeDrift(ACTOR *actor)
{
	actor->x.fixed += actor->tmr << 8;
	actor->y.fixed += toFIXED(1) / 2;
	actor->scaleX += toFIXED(1) / 50;
	actor->scaleY += toFIXED(1) / 50;
	actor->tmr += 2;
	if(--*(Sint32 *)&actor->watchedActor == 0)
		fdActorRelease(actor);
}
