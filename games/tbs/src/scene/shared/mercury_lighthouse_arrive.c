/* Mercury Lighthouse and its entrance: arriving on a floor through one of its
   sparkling passages. */

/* Unless the party returns to where it was, the leader appears 16 pixels north
   of the entrance, hidden, as the screen opens, then comes south with the trail
   of sparks and is drawn again on the way. */
static void scMercuryLighthouseArriveWithSparks(void)
{
	ACTOR *actor;
	SPR	  *spr;

	actor = fdActorGetById(ACTOR_PARTY_LEADER);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		fdScriptStart();
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		actor->motionFlg = 0;
		fdActorSetPosFX(ACTOR_PARTY_LEADER, toFIXED(actor->x.part[0].integer),
			toFIXED(actor->z.part[0].integer) + -toFIXED(16));
		fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
		fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		sySndPlay(SE_SPARKLING_PASSAGE);
		actor->update = (void (*)(ACTOR *))scMercuryLighthouseTrailSparks;
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
		fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
		fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
		spr = actor->spr;
		spr->image.pri = 1;
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->update = NULL;
		fdPlayQueuedMusic();
		fdScriptEnd();
	}
}
