/* Whether flame thrower `idx` is active. */
UNUSED Sint32 scFlameThrowerIsActive(Sint32 idx)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	return (state->entries + idx)->active;
}
