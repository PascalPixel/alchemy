/* Lets an actor fall, waits up to sixty ticks for it to land, plays the sound
   (when not 0) and holds it still ten ticks later: a pushed half face or Felix
   in the Ankohl Ruins, a pillar of Gaia Rock's first cliffs. */
void scActorWaitToLandWithSound(Sint32 id, Sint32 sound)
{
	Sint32 ticks = 60;
	ACTOR *actor = fdActorGet(id);

	actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	while(ticks != 0) {
		syWait(1);
		ticks--;
		if(actor->velocityY == 0)
			break;
	}
	if(sound != 0)
		fdScriptPlaySound(sound);
	syWait(10);
	actor->motionFlags = 0;
}
