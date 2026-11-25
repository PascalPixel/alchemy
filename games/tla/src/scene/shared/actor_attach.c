/// Puts actor `id` on actor `sourceId` and starts its script; at the script's
/// end it launches actor `launchedId` ahead of it, facing `launchFacing`
/// (Actor_Park).
void Actor_Attach(s32 sourceId, s32 id, s32 launchedId, s32 launchFacing,
                  const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *actor = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(actor, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(actor, script);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  Actor_SelectAnimation(actor, 1);
  actor->watchedActor = (union FieldObject *)source;
  actor->timer = 0;
  actor->userValue = launchFacing;
  actor->launchedActorId = launchedId;
  actor->update = Actor_HoldDuringEvent;
}
