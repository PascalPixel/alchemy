/// Stops an actor and starts its scripted puff.
void Actor_StartScripted(struct Actor *actor, s32 priority) {
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  ((struct FieldSprite *)actor->sprite)->priority = priority;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, 2);
  Actor_StartScript(actor, gTaopoSwamp_PuffRises);
  FieldActor_SetSpriteColor(actor, 10);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}
