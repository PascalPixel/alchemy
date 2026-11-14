/// Stops an actor where it stands.
void Actor_Freeze(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    Actor_SetSpriteFlags(object, 0);
    object->collisionFlags = 0;
    object->motionFlags = 0;
  }
}
