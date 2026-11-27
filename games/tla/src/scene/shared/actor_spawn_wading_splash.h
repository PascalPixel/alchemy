// A splash at the actor's feet as it wades, on its sprite's priority.
void scActorSpawnWadingSplash(ACTOR *actor)
{
    ACTOR     *child = scFieldActorCreate(SPRITE_WADING_SPLASH,
                                      actor->x.fixed,
                                      actor->y.fixed + toFIXED(4),
                                      actor->z.fixed + toFIXED(4));
    FIELD_SPR *src   = actor->sprite;

    if (child != 0)
    {
        FIELD_SPR *sprite   = child->sprite;

        child->groundHeight = actor->groundHeight;
        fdActorStartScript(child, WadingSplashScript);
        child->motionFlags = 0;
        if (sprite != 0)
        {
            fdSpriteStartAnimation(sprite, 1);
            sprite->flags    = 0;
            sprite->priority = src->priority;
        }
    }
}
