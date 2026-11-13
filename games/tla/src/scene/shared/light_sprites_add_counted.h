/// LightSprites_Add with the count kept in a plain global.
void LightSprites_AddCounted(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    object->timer = value;
    gLightSpriteActors[gLightSpriteCount] = object;
    gLightSpriteCount++;
  }
}
