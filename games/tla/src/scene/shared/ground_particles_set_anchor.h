// Lets the rain fall about actor `id`, or, for 0, nowhere on screen.
void scGroundParticlesSetAnchor(Sint32 id)
{
    GROUND_PARTICLE_STATE *state = RamHeapSlots->weather;
    ACTOR                 *actor = fdActorGetById(id);

    state->leader                = NULL;
    if (id != 0 && actor != 0)
        state->leader = &actor->x.fixed;
}
