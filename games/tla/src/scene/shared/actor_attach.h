/// Attaches an actor to another with a script and an update.
void Actor_Attach(s32 id, s32 target, s32 kind, s32 a3, const void *script) {
  struct Actor *source = Actor_Get(id);
  struct Actor *object = Actor_Get(target);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedObject = source;
  object->timer = 0;
  object->unk66 = a3;
  BYTE_AT(object, 98) = kind;
  object->update = Actor_HoldDuringEvent;
}
