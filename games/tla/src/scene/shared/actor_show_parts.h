/// Shows an actor's parts.
void Actor_ShowParts(s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (actor != 0) {
    u8 *meta = Sprite_AddAnimation(actor->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Actor_SelectAnimation(actor, 0);
    Actor_SelectAnimation(actor, 2);
  }
}
