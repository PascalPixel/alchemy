/* Turns flame thrower `idx` on or off. */
UNUSED void scFlameThrowerSetActive(Sint32 idx, Sint16 value)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	(state->entries + idx)->active = value;
}
