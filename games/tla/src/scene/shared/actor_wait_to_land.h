/* Lets an actor fall, waits up to sixty frames for it to land and ten more,
   then holds it still. */
void scActorWaitToLand(Sint32 id)
{
	Sint32 remainingFrames = 60;
	ACTOR *actor = fdActorGet(id);

	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	while(remainingFrames != 0) {
		syWait(1);
		if(actor->velY == 0)
			break;
		remainingFrames--;
	}
	syWait(10);
	actor->motionFlg = 0;
}
