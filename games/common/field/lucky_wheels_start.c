/* fdLuckyWheelsStart, the same in both games. Both games'
   field/lucky_wheels.c include it where the function stands. */

/* Plays Lucky Wheels: blanks the screen, seeds the random numbers with the play
   time and runs the game. Returns 0. */
Sint32 fdLuckyWheelsStart(void)
{
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
	RandomSeed = MainRoutineStateWork.playTime;
	sySndPlayStub(MUS_TRY_YOUR_LUCK);
	fdLuckyWheelsRun();
	return 0;
}
