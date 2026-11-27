/* Stops the leader while an action waits. */
void fdLeaderStopIfPending(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(state->busy != 0) {
		fdEventSetFlag(FLAG_MENU_OPEN);
		fdLeaderStopMoving();
		fdEventEndAllLastingPsynergy();
		fdEventClearFlag(FLAG_MENU_OPEN);
	}
}
