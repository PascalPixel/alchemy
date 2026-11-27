/* gmPlayMusic, the same in both games. Both games' game/main_routine.c
   include it where the function stands. */

/* Starts the current scene's field music, chosen by fdChooseMusic. */
void gmPlayMusic(void)
{
	sySndPlayStub(MainRoutineStateWork.sceneMusic);
}
