// The distance between two actors, for the scenes where someone reacts to who is near: the Mercury
// and Venus Lighthouse aeries, Tolbi, Vault, Venus Lighthouse, Xian and every scene with
// pushed_actor.c.

// The distance in whole pixels between two positions, each given by its x and read on through the y
// and z that follow it, as an actor holds them. UB: an actor's x, y and z are separate members,
// read here as an array.
UNUSED static Sint32 scPosDistance(const Sint32 *first, const Sint32 *second)
{
    Sint32 deltaX = FIXEDtoINT(*first++ - *second++);
    Sint32 deltaY = FIXEDtoINT(*first++ - *second++);
    Sint32 deltaZ = FIXEDtoINT(*first - *second);

    return sySquartIWRAM(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

// The distance in whole pixels between two actors, in all three axes.
#define fdActorDistance(first, second) scPosDistance(&(first)->x.fixed, &(second)->x.fixed)
