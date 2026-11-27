/* mnMessageArgumentLookup, the same in both games. Both games'
   window/window_messages.c include it where the function stands. */

/* Returns the value of the first message argument of kind (MESSAGE_ARGUMENT_),
   or 0 when there is none. */
Uint32 mnMessageArgumentLookup(Uint32 kind, Uint32 clear)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   i;
	Uint32	   value = 0;

	for(i = 0; i < MESSAGE_ARGUMENT_COUNT; i++) {
		if(state->argumentKinds[i] == kind) {
			value = state->argumentValues[i];
			if(clear != 0) {
				state->argumentValues[i] = 0;
				state->argumentKinds[i] = MESSAGE_ARGUMENT_NONE;
			}
			break;
		}
	}
	return value;
}
