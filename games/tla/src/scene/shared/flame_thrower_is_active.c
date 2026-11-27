/* Whether flame thrower `index` is active. */
UNUSED Sint32 scFlameThrowerIsActive(Sint32 index)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	return (state->entries + index)->active;
}
