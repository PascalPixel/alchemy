/// Stops an actor and starts its scripted puff.
void Actor_StartScripted(struct Actor *object, s32 priority) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  ((struct FieldSprite *)object->sprite)->priority = priority;
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, 2);
  Actor_StartScript(object, gTaopoSwamp_PuffRises);
  FieldActor_SetSpriteColor(object, 10);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
