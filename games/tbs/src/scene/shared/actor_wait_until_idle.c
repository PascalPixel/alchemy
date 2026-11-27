// Babi Lighthouse and Venus Lighthouse.

// Waits until the actor stops rising or falling, a second at most.
static void scActorWaitUntilIdle(ACTOR *actor)
{
    Sint32 ticks;
    Sint32 busy;

    ticks = TICKS_PER_SECOND;
    while (ticks != 0)
    {
        sySynch(1);
        busy = actor->velocityY;
        ticks--;
        if (busy == 0)
            break;
    }
}
