/* Lets an actor fall, waits up to sixty frames for it to land, plays the sound
   (when not 0) and holds it still ten frames later: a pushed half face or Felix
   in the Ankohl Ruins, a pillar of Gaia Rock's first cliffs. */
void scActorWaitToLandWithSnd(Sint32 id, Sint32 snd)
{
	Sint32 frames = 60;
	ACTOR *actor = fdActorGet(id);

	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	while(frames != 0) {
		syWait(1);
		frames--;
		if(actor->velY == 0)
			break;
	}
	if(snd != 0)
		fdScriptPlaySnd(snd);
	syWait(10);
	actor->motionFlg = 0;
}
