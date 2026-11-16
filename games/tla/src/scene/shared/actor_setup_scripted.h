/// Stops an actor and starts its script, part 2 and colour 10.
void Actor_SetupScripted(struct Actor *actor) {
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, 2);
  Actor_StartScript(actor, gScriptedPart_Script);
  FieldActor_SetSpriteColor(actor, 10);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}
