/* btDjinnRecoveryCountDownTwenty and btTurnEndNoOperation, the same in both
   games. Both games' battle/battle_turn_end.c include it where the functions
   stand. */

/* Runs btDjinnRecoveryCountDown twenty times, which sets every recovering
   Djinni of a party member with HP; those on standby stay so. Returns 0. */
Sint32 btDjinnRecoveryCountDownTwenty(void)
{
	Sint32 turn;

	for(turn = 0; turn < 20; turn++)
		btDjinnRecoveryCountDown();
	return 0;
}

/* Does nothing; btRun calls it at the end of each turn and of the battle. */
void btTurnEndNoOperation(void)
{
}
