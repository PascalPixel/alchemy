/// Stops an actor and starts its script, its sprite drawn at `priority`.
void Actor_SetupScriptedPart(struct Actor *object, s32 priority) {
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteAttr2High *)object->sprite)->priority = priority;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gScriptedPart_Script);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}
