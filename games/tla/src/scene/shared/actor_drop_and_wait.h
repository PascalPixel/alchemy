// Lets an actor fall to the ground and waits there: a thump as it lands, ten ticks, and it stands
// still again.
void fdActorDropAndWait(ACTOR *actor)
{
    Uint8 *flags = &actor->motionFlags;

    *flags       = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    do
    {
        sySynch(1);
    } while (actor->velocityY != 0);
    sySoundPlay(SE_THUMP);
    sySynch(10);
    *flags = 0;
}
