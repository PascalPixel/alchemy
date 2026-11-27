// Stops the leader while an action waits.
void fdLeaderStopIfPending(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	if (state->busy != 0)
	{
		fdScriptSetFlag(FLAG_MENU_OPEN);
		fdLeaderStopMoving();
		fdScriptEndAllLastingPsynergy();
		fdScriptClearFlag(FLAG_MENU_OPEN);
	}
}
