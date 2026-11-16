/// Puts actor `id` on actor `sourceId`, to send `value` rolling at `angle`
/// when its script says.
void Actor_Attach(s32 sourceId, s32 id, s32 value, s32 angle,
                  const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *actor = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(actor, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(actor, script);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  Actor_SelectAnimation(actor, 1);
  actor->watchedActor = source;
  actor->timer = 0;
  actor->userValue = angle;
  actor->launchedActorId = value;
  actor->update = Actor_HoldDuringEvent;
}
