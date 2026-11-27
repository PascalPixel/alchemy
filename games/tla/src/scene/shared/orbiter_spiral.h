// Turns an orbiting object by half its speed and puts it out from its anchor, raised 8 pixels, by
// its radius.
static inline void scOrbiterStep(ORBITER *orbiter, ACTOR *anchor)
{
    orbiter->angle += (Sint16)orbiter->speed >> 1;
    orbiter->x      = anchor->x.fixed;
    orbiter->y      = anchor->y.fixed + toFIXED(8);
    orbiter->z      = anchor->z.fixed;
    syFieldVectorAddPolarOffsetFX(orbiter->orbitRadius, orbiter->angle, &orbiter->x);
}

// Steps an orbiting object round its anchor: returns 0 once its countdown runs out; else, unless
// its radius is down to 2 pixels, it leaves a spark, closes 4 pixels and steps again, leaving
// another and closing 4 more. Returns 1 while it runs.
Bool syOrbiterSpiral(ORBITER *orbiter)
{
    ACTOR *anchor = orbiter->anchor;

    scOrbiterStep(orbiter, anchor);
    if (--orbiter->countdown == 0)
        return FALSE;
    if (orbiter->orbitRadius == toFIXED(2))
        return TRUE;
    scOrbitSparkSpawn(orbiter);
    orbiter->orbitRadius -= toFIXED(4);
    scOrbiterStep(orbiter, anchor);
    scOrbitSparkSpawn(orbiter);
    orbiter->orbitRadius -= toFIXED(4);
    return TRUE;
}
