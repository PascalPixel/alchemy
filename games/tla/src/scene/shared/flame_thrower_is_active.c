/* Whether flame thrower `index` is active. */
UNUSED s32 scFlameThrowerIsActive(s32 index) {
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	return (state->entries + index)->active;
}
