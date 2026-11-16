/// Stops an actor and starts its script, its sprite drawn at `priority`.
void Actor_SetupScriptedPart(struct Actor *actor, s32 priority) {
  s32 zero = 0;

  actor->motionFlags = zero;
  actor->collisionFlags = zero;

  ((struct SpriteAttr2High *)actor->sprite)->priority = priority;
  Actor_SetSpriteFlags(actor, 0);
  Actor_StartScript(actor, gScriptedPart_Script);
  actor->timer = zero;
  actor->speedLimit = zero;
  actor->acceleration = zero;
}
