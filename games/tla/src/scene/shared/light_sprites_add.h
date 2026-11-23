/// Adds an actor to the glow list, its glow showing `frame`.
void LightSprites_Add(s32 id, s32 frame) {
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL) {
    s16 *count;

    actor->timer = frame;
    count = &gLightSpriteCount;
    gLightSpriteActors[*count] = actor;
    (*count)++;
  }
}
