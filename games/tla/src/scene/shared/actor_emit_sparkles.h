// Sparkles rising about an actor, as about an Adept a tablet has taught Psynergy: every eighth tick
// a white sparkle starts within 16 pixels to either side of it, 8 to 39 pixels up, and rises half a
// pixel a tick.
void scActorEmitSparkles(ACTOR *actor)
{
    if ((TickCount & 7) == 0)
    {
        EFFECT_OPTIONS sparkle;
        FIXED          pos[3];

        sparkle.palette  = SPRITE_COLOR_WHITE;
        sparkle.type     = SPRITE_SPARKLE;
        sparkle.priority = 1;
        pos[0]           = actor->x.fixed + toFIXED((syRandom() & 31) - 16);
        pos[1]           = actor->y.fixed + toFIXED(syRandom() & 31) + toFIXED(8);
        pos[2]           = actor->z.fixed;
        scFieldEffectSpawn(pos[0],
                           pos[1],
                           pos[2] + toFIXED(4),
                           0,
                           toFIXED(0.5),
                           0,
                           EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                           &sparkle);
    }
}
