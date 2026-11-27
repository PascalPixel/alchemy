/* Timber Rooms A (Suhalla) and Timber Rooms B and C (Lalivero). */

/* The party leaves by the touched exit, with the exit sound: the exit numbered
   as the step cue the party walked into. */
static void scLeaveByExit(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint16		*cue = (Sint16 *)&state->stepCue;

	syImportScriptSetExitRequest(*cue);
	sySndPlay(SE_MAP_EXIT);
}
