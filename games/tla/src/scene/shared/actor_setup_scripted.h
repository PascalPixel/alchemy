/// Stops an actor and starts its script, part 2 and colour 10.
void Actor_SetupScripted(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, 2);
  Actor_StartScript(object, gScriptedPart_Script);
  FieldActor_SetSpriteColor(object, 10);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
