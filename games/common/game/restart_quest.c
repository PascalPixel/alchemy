/* gmRestartQuest, the same in both games. Both games' game/game_units.c
   include it where the function stands. */

/* Starts a new game: clears the main routine's state and restarts at the
   opening. */
void gmRestartQuest(void)
{
	gmMainRoutineStateInitDefaults();
	gmMainRoutineRunStub(0);
}
