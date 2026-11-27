/* Unused: the leader spins round, sinking, with splashes at the place of Aqua
   Rock's Vial chest room's third puddle, and leaves by `entrance`. */
UNUSED void scLeaderSpinAway(Sint32 entrance)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	leader->motionFlg = 0;
	leader->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdScriptPlaySnd(SE_SPLASH_SMALL);
	fdActorSelectAnimation(leader, ANIM_LEADER_TUMBLE_IN_WATER);
	fdScriptPlaySnd(SE_SPIN_AWAY);
	for(i = 0; i < 60; i++) {
		Sint32 ang;

		if(i == 30)
			fdScriptFinishScreenTransition();
		leader->y.fixed -= toFIXED(1) / 4;
		ang = leader->facing;
		leader->facing = ang + TRIG_TURN / 16;
		if((i & 7) == 0)
			scActorSpawnWadingSplash(fdActorGet(ACTOR_VIAL_CHEST_PUDDLE_C));
		syWait(1);
	}
	fdScriptEnd();
	syFieldScriptSetExitRequest(entrance);
}
