// Sets a vent's actor.
void scVentSetId(Sint32 i, Sint32 id)
{
    VENT_WORK *state = VENT_STATE;

    if ((Uint32)i <= 3)
        state->ids[i] = id;
}
