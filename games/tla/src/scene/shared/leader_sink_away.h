/* The leader walks 8 pixels north in a trail of mist, then turns blue and loses
   his shadow as the screen closes, and leaves by exit `entrance`, with the
   sparkling sound of a hidden way: Aqua Rock's and the Apojii Islands' passages
   through water. scLeaderRiseFromWater is the arrival. */
void scLeaderSinkAway(Sint32 entrance)
{
	ACTOR  *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 *owner = &MainRoutineStateWork.leaderActorId;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_SPARKLING_PASSAGE);
	leader->update = (OBJECT_UPDATE)scActorEmitMist;
	leader->motionFlg = 0;
	leader->speedLimit = toFIXED(1) / 5;
	fdActorSetAnimation(*owner, 2);
	fdActorSetDestinationOffset(*owner, 0, -8);
	scFieldActorWaitForMoveById(*owner);
	scFieldActorSetSprCol(fdActorGet(*owner), SPRITE_COLOR_BLUE_TINT);
	fdActorSetSprFlags(fdActorGet(*owner), 0);
	leader->update = 0;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(entrance);
	fdScriptEnd();
}
