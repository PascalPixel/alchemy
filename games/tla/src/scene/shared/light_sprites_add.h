// Adds an actor to the glow list, its glow showing `frame`.
void scLightSpritesAdd(Sint32 id, Sint32 frame)
{
    ACTOR *actor = fdActorGet(id);

    if (actor != NULL)
    {
        Sint16 *count;

        actor->timer              = frame;
        count                     = &LightSpriteCount;
        LightSpriteActors[*count] = actor;
        (*count)++;
    }
}
