/// Actor_ResetScripted as a two-argument callback.
void Actor_ResetScriptedFor(struct Actor *actor, s32 unused) {
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_StartScript(actor, gScriptedPart_Script);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}
