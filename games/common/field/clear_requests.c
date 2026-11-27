/* fdClearRequests, the same in both games. Both games'
   field/field_mode_walking.c include it where the function stands. */

/* Clears the field's twelve pending requests (see FIELD_STATE). */
void fdClearRequests(void)
{
	FIELD_STATE *state = FieldStateWork;

	state->stepCue = 0;
	state->pushMetatile = 0;
	state->exit = 0;
	state->fieldMenu = 0;
	state->aButton = 0;
	state->pauseMenu = 0;
	state->talk = 0;
	state->examineMetatile = 0;
	state->encounter = 0;
	state->psynergy = 0;
	state->item = 0;
	state->raisedCue = 0;
}
