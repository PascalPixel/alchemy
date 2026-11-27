/* @file */
/* Timber Rooms A (Suhalla) and Timber Rooms B and C (Lalivero). */

/* The party leaves by the touched exit, with the exit sound: the exit */
/* numbered as the step trigger the party walked into. */
static void scLeaveByExit(void) {
	FIELD_STATE *state = FieldStateWork;
	s16 *trigger = (s16 *)&state->stepTrigger;

	syImportEventSetExitRequest(*trigger);
	sySndPlay(SE_MAP_EXIT);
}
