/// Shows an actor's parts.
void Actor_ShowParts(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Actor_SelectAnimation(object, 0);
    Actor_SelectAnimation(object, 2);
  }
}
