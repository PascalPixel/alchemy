// A flame thrower's flame (FLAME_EAST), sent east from `pos` at `speed` (scFlameUpdateSideways);
// `offScreen` keeps its sprite's picture loaded out of sight.
void scFlameThrowerSendEast(Sint32 *pos, Sint32 speed, Sint32 offScreen)
{
    ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, pos[0], pos[1], pos[2]);

    if (actor != NULL)
    {
        actor->bounce    = 0;
        actor->gravity   = 0;
        actor->sceneWord = speed;
        scActorSetupScriptedPart(actor, 3);
        actor->update = scFlameUpdateSideways;
        if (offScreen)
            actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
        fdActorSelectAnimation(actor, 2);
    }
}
