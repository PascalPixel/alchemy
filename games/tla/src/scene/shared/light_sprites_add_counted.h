/// LightSprites_Add with the count kept in a plain global.
void LightSprites_AddCounted(s32 id, s32 frame) {
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL) {
    actor->timer = frame;
    gLightSpriteActors[gLightSpriteCount] = actor;
    gLightSpriteCount++;
  }
}
