/* Unused: sets the actor the flames check against in slot `index`. */
UNUSED void scFlameThrowersSetStop(u32 index, s32 value) {
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	if(index <= 3)
		state->slots[index].id = value;
}
