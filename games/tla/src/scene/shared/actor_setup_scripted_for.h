/// Actor_SetupScripted as a two-argument callback.
void Actor_SetupScriptedFor(struct Actor *actor, s32 unused) {
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
