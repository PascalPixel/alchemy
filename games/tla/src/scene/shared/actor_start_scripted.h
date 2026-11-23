/// Readies a vent's puff: it stops, is drawn behind the actors around it, its
/// sprite at OBJ `priority`, shadowless and tinted SPRITE_COLOR_RED_TINT, and
/// breaks up (the dust cloud's animation 2) without spinning or growing until
/// gDust_BurstScript releases it, 44 frames on.
void Actor_StartScripted(struct Actor *actor, s32 priority) {
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  ((struct FieldSprite *)actor->sprite)->priority = priority;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, 2);
  Actor_StartScript(actor, gDust_BurstScript);
  FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}
