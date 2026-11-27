/* Unused: the leader spins round, sinking, with splashes at the place of */
/* Aqua Rock's Vial chest room's third puddle, and leaves by `entrance`. */
UNUSED void fdLeaderSpinAway(s32 entrance) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 i;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	leader->motionFlg = 0;
	leader->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdEventPlaySnd(SE_SPLASH_SMALL);
	fdActorSelectAnimation(leader, ANIM_LEADER_TUMBLE_IN_WATER);
	fdEventPlaySnd(SE_SPIN_AWAY);
	for(i = 0; i < 60; i++) {
		s32 angle;

		if(i == 30)
			fdEventFinishScrTransition();
		leader->y.fixed -= toFIXED(1) / 4;
		angle = leader->facing;
		leader->facing = angle + TRIG_TURN / 16;
		if((i & 7) == 0)
			fdActorSpawnWadingSplash(fdActorGet(ACTOR_VIAL_CHEST_PUDDLE_C));
		syWait(1);
	}
	fdEventEnd();
	syFieldEventSetExitRequest(entrance);
}
