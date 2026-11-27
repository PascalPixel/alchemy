// Stops an actor and starts its script, its sprite drawn at `priority`.
void scActorSetupScriptedPart(ACTOR *actor, Sint32 priority)
{
    actor->motionFlags                          = 0;
    actor->collisionFlags                       = 0;

    ((SPR_ATTR2_HIGH *)actor->sprite)->priority = priority;
    fdActorSetSpriteFlags(actor, 0);
    fdActorStartScript(actor, ScriptedPartScript);
    actor->timer        = 0;
    actor->speedLimit   = 0;
    actor->acceleration = 0;
}
