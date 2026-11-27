// Goma Cave Entrance and Goma Cave, and the Lamakan Desert.

// Waits until the actor has come down to height (16.16), a second at most.
static void scActorWaitBelowHeight(ACTOR *actor, FIXED height)
{
    Sint32 ticks = 60;

    while (ticks != 0)
    {
        sySynch(1);
        ticks--;
        if (actor->y.fixed <= height)
            break;
    }
}
