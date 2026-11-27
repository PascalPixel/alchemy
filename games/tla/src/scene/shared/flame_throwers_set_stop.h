/* Unused: sets the actor the flames check against in slot `idx`. */
UNUSED void scFlameThrowersSetStop(Uint32 idx, Sint32 value)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	if(idx <= 3)
		state->slots[idx].id = value;
}
