// Cottage Rooms B and the world map, where nothing reaches it.

// Unused: an actor's update function that steps it through the six colour modes from
// SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG, one every two ticks.
UNUSED static void scCycleColorModesOnOddTicks(ACTOR *actor)
{
    if ((TickCount & 1) != 0)
    {
        Sint32 slot = (TickCount >> 1) % 6;

        fdActorSetColorMode(actor, slot);
    }
}
