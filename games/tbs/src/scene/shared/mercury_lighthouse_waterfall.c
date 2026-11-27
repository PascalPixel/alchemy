/* @file */
/* Mercury Lighthouse and its entrance: leaving a room by walking into one of */
/* its waterfalls. */

/* Walking into a waterfall: the leader moves north with the trail of sparks, */
/* vanishes, and after half a second the screen fades and the party takes that
 */
/* exit. */
static void scMercuryLighthouseWalkIntoWaterfall(s32 exit) {
	ACTOR *leader = fdActorGetById(ACTOR_PARTY_LEADER);
	fdEventStart();
	sySndPlay(SE_SPARKLING_PASSAGE);
	leader->update = (void (*)(ACTOR *))scMercuryLighthouseTrailSparks;
	leader->speedLimit = toFIXED(1) / 5;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffsetById(ACTOR_PARTY_LEADER, 0, -6);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorConfigureSprCol(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGetById(ACTOR_PARTY_LEADER), 0);
	leader->update = NULL;
	fdEventWait(30);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(exit);
	fdEventEnd();
}

/* XXX (no evidence): whether this is an early form of */
/* scMercuryLighthouseWalkIntoWaterfall, which plays the same sound and steps */
/* the same way; nothing calls it, so nothing shows what would have followed. */
/* Unused: the leader steps 8 pixels north with the waterfalls' sound and */
/* vanishes, then is put at x 8 * column + 8, z 0, with no exit and no end to */
/* the event; nothing in either overlay calls it. */
static void UNUSED scMercuryLighthouseDropLeaderToColumn(s32 column) {
	fdEventStart();
	sySndPlay(SE_SPARKLING_PASSAGE);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	fdActorSetDestinationOffsetById(ACTOR_PARTY_LEADER, 0, -8);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdEventWait(8);
	fdActorSetPosFX(ACTOR_PARTY_LEADER,
					FIELD_HALF_CELL_ORIGIN(column) + toFIXED(8), 0);
	fdEventWait(30);
}
