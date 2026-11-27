// Unless the party returns to where it was, the leader appears in the waterfall 16 pixels north of
// the entrance, tinted blue, as the screen opens, then comes south out of it in a trail of mist and
// takes his own colours and shadow back on the way.
void scLeaderRiseFromWater(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	if (!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
	{
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdCameraMoveToFX(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		leader->motionFlags = 0;
		fdActorSetPosFX(MainRoutineStateWork.leaderActorId,
		                toFIXED(leader->x.part[0].integer),
		                toFIXED(leader->z.part[0].integer) - toFIXED(16));
		scFieldActorSetSpriteColor(fdActorGet(MainRoutineStateWork.leaderActorId),
		                           SPRITE_COLOR_BLUE_TINT);
		fdActorSetSpriteFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
		fdScriptStartScreenTransition();
		fdScriptPlaySound(SE_SPARKLING_PASSAGE);
		leader->update = (OBJECT_UPDATE)scActorEmitMist;
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.4);
		fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 0, 8);
		scFieldActorSetSpriteColor(fdActorGet(MainRoutineStateWork.leaderActorId),
		                           SPRITE_COLOR_NORMAL);
		fdActorSetSpriteFlags(fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_FLAG_SHADOW);
		((FIELD_SPR *)leader->sprite)->priority = 1;
		fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 0, 10);
		leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->update      = NULL;
		fdScriptNoOperation();
		fdScriptWaitTransitionTicks();
		fdScriptEnd();
	}
}
