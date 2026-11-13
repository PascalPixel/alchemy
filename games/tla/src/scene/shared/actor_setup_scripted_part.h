/// Stops an actor and starts its script with sprite part `part`.
void Actor_SetupScriptedPart(struct Actor *object, s32 part) {
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gData_020056a0);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}
