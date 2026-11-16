/// Adds an actor to the glow list, its frame in `value`.
void LightSprites_Add(s32 id, s32 value) {
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL) {
    s16 *count;

    actor->timer = value;
    count = &gLightSpriteCount;
    gLightSpriteActors[*count] = actor;
    (*count)++;
  }
}
