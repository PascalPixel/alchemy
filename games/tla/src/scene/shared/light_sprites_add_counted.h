/// LightSprites_Add with the count kept in a plain global.
void LightSprites_AddCounted(s32 id, s32 value) {
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL) {
    actor->timer = value;
    gLightSpriteActors[gLightSpriteCount] = actor;
    gLightSpriteCount++;
  }
}
