/// Stops an actor and starts its script.
void Actor_ResetScripted(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gScriptedPart_Script);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
