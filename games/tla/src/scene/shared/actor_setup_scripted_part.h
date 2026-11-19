/// Stops an actor and starts its script, its sprite drawn at `priority`.
void Actor_SetupScriptedPart(struct Actor *actor, s32 priority) {
  actor->motionFlags = 0;
  actor->collisionFlags = 0;

  ((struct SpriteAttr2High *)actor->sprite)->priority = priority;
  Actor_SetSpriteFlags(actor, 0);
  Actor_StartScript(actor, gScriptedPart_Script);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}
