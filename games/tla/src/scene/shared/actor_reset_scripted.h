/// Stops an actor and starts its script.
void Actor_ResetScripted(struct Actor *actor) {
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_StartScript(actor, gScriptedPart_Script);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}
