/// Actor_ResetScripted as a two-argument callback.
void Actor_ResetScriptedFor(struct Actor *object, s32 unused) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gScriptedPart_Script);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
