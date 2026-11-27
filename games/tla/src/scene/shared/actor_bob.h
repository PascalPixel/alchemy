// scActorBob's cycle, in ticks.
#define BOB_CYCLE_TICKS  64
// Its sprite's rotation is the cosine of its swing over this: up to a 64th of a turn either way.
#define BOB_ROCK_DIVISOR 64

// An actor's script call that never ends: it hops three times every BOB_CYCLE_TICKS ticks, at the
// cycle's start, at tick 16 and at tick 24, while its sprite rocks from side to side, once every 16
// ticks.
Sint32 scActorBob(ACTOR *actor)
{
    Sint32     frame   = actor->sceneCount & (BOB_CYCLE_TICKS - 1);
    FIELD_SPR *sprite  = actor->sprite;

    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    if (frame == 0)
        actor->velocityY = toFIXED(3);
    if (frame == 16)
        actor->velocityY = toFIXED(2);
    if (frame == 24)
        actor->velocityY = toFIXED(3);
    sprite->rotation = syCos(frame * (TRIG_TURN / 16)) / BOB_ROCK_DIVISOR;
    actor->sceneCount++;
    return -1;
}

#undef BOB_CYCLE_TICKS
#undef BOB_ROCK_DIVISOR
