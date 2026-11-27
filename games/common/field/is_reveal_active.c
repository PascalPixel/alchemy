/* fdIsRevealActive, the same in both games. Both games'
   field/field_mode_item_display.c include it where the function stands. */

/* Returns 1 while Reveal is active, else 0. */
Bool fdIsRevealActive(ACTOR *interpreter)
{
	FIELD_STATE *state = FieldStateWork;

	if(state == NULL)
		return FALSE;
	if(state->revealActive != 0)
		return TRUE;
	else
		return FALSE;
}
