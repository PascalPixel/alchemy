/// Adds an actor to the glow list, its frame in `value`.
void LightSprites_Add(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gLightSpriteCount;
    gLightSpriteActors[*count] = object;
    (*count)++;
  }
}
