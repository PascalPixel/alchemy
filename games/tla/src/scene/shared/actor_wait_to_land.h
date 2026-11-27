/* Lets an actor fall, waits up to sixty ticks for it to land and ten more,
   then holds it still. */
void scActorWaitToLand(Sint32 id)
{
	Sint32 remainingTicks = 60;
	ACTOR *actor = fdActorGet(id);

	actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	while(remainingTicks != 0) {
		syWait(1);
		if(actor->velocityY == 0)
			break;
		remainingTicks--;
	}
	syWait(10);
	actor->motionFlags = 0;
}
