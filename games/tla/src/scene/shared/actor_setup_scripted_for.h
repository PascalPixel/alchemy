/// Readies a flame thrower's spark (FLAME_SPARK): it stops, shadowless and
/// tinted SPRITE_COLOR_RED_TINT, and breaks up (the dust cloud's animation 2)
/// without spinning or growing until gScriptedPart_Script releases it, half a
/// second on.
void Actor_SetupScriptedFor(struct Actor *actor, s32 unused) {
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, 2);
  Actor_StartScript(actor, gScriptedPart_Script);
  FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}
