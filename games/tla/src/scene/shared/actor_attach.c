/// Puts actor `id` on actor `sourceId`, to send `value` rolling at `angle`
/// when its script says.
void Actor_Attach(s32 sourceId, s32 id, s32 value, s32 angle,
                  const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedActor = source;
  object->timer = 0;
  object->userValue = angle;
  object->launchedActorId = value;
  object->update = Actor_HoldDuringEvent;
}
