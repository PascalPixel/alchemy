// Sets flame thrower `index`'s flame (FLAME_THROWER_FLAGS) and reach.
void scFlameThrowerSet(Sint32 index, Sint32 value)
{
    FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
    FLAME_THROWER   *entry = &state->entries[index];

    entry->reach           = toFIXED(value & FLAME_THROWER_REACH_BITS);
    entry->flags           = value & FLAME_THROWER_FLAG_BITS;
}
