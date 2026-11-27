// 1 when the leader faces north, within one eighth of a turn.
Sint32 scLeaderIsFacingNorth(void)
{
	Uint16 turn;
	Sint32 facing;

	turn   = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing + (DEGtoANG(135) - 1);
	facing = 1;
	if (turn >= DEGtoANG(90) - 1)
		facing = 0;
	return facing;
}
