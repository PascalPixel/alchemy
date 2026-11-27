/* Unused: the leader spins round, sinking, with splashes at the place of Aqua
   Rock's Vial chest room's third puddle, and leaves by `entrance`. */
UNUSED void scLeaderSpinAway(Sint32 entrance)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 1);
	leader->motionFlags = 0;
	leader->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
	fdScriptPlaySound(SE_SPLASH_SMALL);
	fdActorSelectAnimation(leader, ANIM_LEADER_TUMBLE_IN_WATER);
	fdScriptPlaySound(SE_SPIN_AWAY);
	for(i = 0; i < 60; i++) {
		Sint32 angle;

		if(i == 30)
			fdScriptFinishScreenTransition();
		leader->y.fixed -= toFIXED(1) / 4;
		angle = leader->facing;
		leader->facing = angle + TRIG_TURN / 16;
		if((i & 7) == 0)
			scActorSpawnWadingSplash(fdActorGet(ACTOR_VIAL_CHEST_PUDDLE_C));
		syWait(1);
	}
	fdScriptEnd();
	syFieldScriptSetExitRequest(entrance);
}
