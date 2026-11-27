/* 1 when the leader faces north, within one eighth of a turn. */
s32 fdLeaderIsFacingNorth(void) {
	u16 turn;
	s32 facing;

	turn = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing +
		   (DEGtoANG(135) - 1);
	facing = 1;
	if(turn >= DEGtoANG(90) - 1)
		facing = 0;
	return facing;
}
