/* Sets flame thrower `index`'s flame (FLAME_THROWER_FLAGS) and reach. */
void scFlameThrowerSet(s32 index, s32 value) {
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	FLAME_THROWER *entry = &state->entries[index];

	entry->reach = INTtoFIXED(value & FLAME_THROWER_REACH_BITS);
	entry->flg = value & FLAME_THROWER_FLAG_BITS;
}
