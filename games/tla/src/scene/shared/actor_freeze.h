/// Stops an actor where it stands.
void Actor_Freeze(s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL) {
    Actor_SetSpriteFlags(actor, 0);
    actor->collisionFlags = 0;
    actor->motionFlags = 0;
  }
}
