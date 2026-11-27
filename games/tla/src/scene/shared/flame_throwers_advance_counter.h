// Unused: steps the flame throwers' counter while the party walks, wrapping at `limit`, as the
// vents' scVentAdvanceCounter does.
UNUSED Sint32 scFlameThrowersAdvanceCounter(Sint32 limit)
{
    FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

    if (!fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS))
    {
        if (++state->count >= limit)
            state->count = 0;
    }
    return state->count;
}
