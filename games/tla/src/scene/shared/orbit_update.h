// An orbiting actor's growth climbs by ORBIT_GROWTH_STEP every other tick, up to ORBIT_GROWTH_FULL,
// and its circle's radius is the growth times the sine of its phase, in ORBIT_PHASE_UNIT steps.
// Once grown it slows by ORBIT_SLOWDOWN a tick to a stop.
#define ORBIT_GROWTH_FULL 141
#define ORBIT_GROWTH_STEP 10
#define ORBIT_PHASE_UNIT  (TRIG_TURN / 64)
#define ORBIT_SLOWDOWN    128

// Moves an orbiting actor round its centre as its circle grows, on a circle squashed to half its
// depth; returns 0 once it has stopped.
Bool syOrbitUpdate(ORBIT *object)
{
    FIXED z;

    object->angle += object->speed;
    object->x      = syIWRAMMulFX(syCos(object->angle), object->orbitRadius);
    z              = syIWRAMMulFX(syMathSin(object->angle), object->orbitRadius);
    object->x     += object->centreX;
    object->z      = (z >> 1) + object->centreZ;
    if (object->growth != ORBIT_GROWTH_FULL && (TickCount & 1))
    {
        object->orbitRadius  = object->growth * syMathSin(object->phase * ORBIT_PHASE_UNIT);
        object->growth      += ORBIT_GROWTH_STEP;
    }
    if (object->speed == 0)
        return FALSE;
    if (object->growth == ORBIT_GROWTH_FULL)
        object->speed -= ORBIT_SLOWDOWN;
    return TRUE;
}

#undef ORBIT_GROWTH_FULL
#undef ORBIT_GROWTH_STEP
#undef ORBIT_PHASE_UNIT
#undef ORBIT_SLOWDOWN
