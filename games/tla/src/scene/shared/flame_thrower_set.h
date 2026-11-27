/* Sets flame thrower `idx`'s flame (FLAME_THROWER_FLAGS) and reach. */
void scFlameThrowerSet(Sint32 idx, Sint32 value)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	FLAME_THROWER	*entry = &state->entries[idx];

	entry->reach = toFIXED(value & FLAME_THROWER_REACH_BITS);
	entry->flg = value & FLAME_THROWER_FLAG_BITS;
}
