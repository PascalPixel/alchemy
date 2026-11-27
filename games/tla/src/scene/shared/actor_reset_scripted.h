// Stops an actor and starts its script.
void scActorResetScripted(struct ACTOR *actor)
{
    actor->motionFlags    = 0;
    actor->collisionFlags = 0;
    fdActorSetSpriteFlags(actor, 0);
    fdActorStartScript(actor, ScriptedPartScript);
    actor->timer        = 0;
    actor->speedLimit   = 0;
    actor->acceleration = 0;
}
