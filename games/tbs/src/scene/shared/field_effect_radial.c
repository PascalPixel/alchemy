// The Tunnel Ruins and Venus Lighthouse: the ring of dust a carved stone or a pillar raises as it
// lands on a stack.

// Bursts nine white puffs of dust out from the actor in a ring, one each eighth of a turn with the
// ninth on the first, flung a third further east and west, slowing as they go
// (scFieldEffectAdvanceMotion).
static void scFieldEffectSpawnNineRadialEffects(Sint32 actorId)
{
    ACTOR         *actor;
    VECTOR3        velocity;
    EFFECT_OPTIONS options;
    Uint32         sixteenth;
    Sint32         angle;
    Sint32         x;
    FIXED          z;

    actor            = fdActorGetById(actorId);
    options.priority = 1;
    options.palette  = SPRITE_COLOR_WHITE;
    options.update   = scFieldEffectAdvanceMotion;
    for (sixteenth = 0; sixteenth <= TRIG_TURN / FACING_STEP; sixteenth += 2)
    {
        angle       = sixteenth * FACING_STEP;
        velocity.x  = syCos(angle);
        velocity.y  = 0;
        z           = sySin(angle);
        x           = velocity.x;
        velocity.z  = z;
        x          += syMathDivide(x, 3);
        velocity.x  = x;
        fdEffectSpawn(actor->x.fixed,
                      actor->y.fixed,
                      actor->z.fixed,
                      x,
                      velocity.y,
                      z,
                      EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE
                          | EFFECT_SCRIPT_MEDIUM,
                      &options);
    }
}
