/// The glow sprites: 16 pixels square, or 32 by 64 with gLightSpritesLarge,
/// in 256 colours, each frame of the glow 8 tiles on from the last. Both stand
/// on their actor, LIGHT_LEFT pixels to its left.
#define LIGHT_SMALL_SIZE 16
#define LIGHT_LARGE_HEIGHT 64
#define LIGHT_LEFT 8

/// Draws a glow sprite over each listed actor.
void LightSprites_Draw(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  tile = gVramCacheEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gLightSpriteOams;

  for (i = 0; i < gLightSpriteCount; i++) {
    struct Actor *actor = gLightSpriteActors[i];
    if (actor != 0 && actor->x.fixed != 0) {
      s32 large, screenX;
      FIXED y, z;
      s32 screenY, depth;
      s32 frame;
      u32 *word;
      screenX = actor->x.fixed - viewX;
      y = actor->y.fixed - viewY + toFIXED(8);
      z = actor->z.fixed - viewZ - viewY;
      screenY = z - y;
      depth = FIXEDtoINT(y + z) + SCENE_SPRITE_DEPTH;
      large = gLightSpritesLarge;
      if (large == 0) {
        s32 actorX, actorY;
        frame = actor->timer;
        actorX = FIXEDtoINT(screenX);
        actorY = FIXEDtoINT(screenY);
        screenX = actorX - LIGHT_LEFT;
        screenY = actorY - LIGHT_SMALL_SIZE;
        if ((u32)(actorX + 7) > 334 || screenY <= -LIGHT_SMALL_SIZE ||
            screenY > 239)
          continue;
        screenX &= OBJ_X_MASK;
        screenY &= OBJ_Y_MASK;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = screenY | OBJ_X(screenX) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
        *word = (tile + frame * 8) | OBJ_PRIORITY(2);
        if (gLightSpritePriority == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 actorX, actorY;
        frame = actor->timer;
        actorX = FIXEDtoINT(screenX);
        actorY = FIXEDtoINT(screenY);
        screenX = actorX - LIGHT_LEFT;
        screenY = actorY - LIGHT_LARGE_HEIGHT;
        if ((u32)(actorX + 23) > 350 || screenY <= -LIGHT_LARGE_HEIGHT ||
            screenY > 175)
          continue;
        screenX &= OBJ_X_MASK;
        screenY &= OBJ_Y_MASK;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = screenY | OBJ_X(screenX) | OBJ_SIZE_32X64 | OBJ_256_COLORS;
        *word = (tile + frame * 8) | OBJ_PRIORITY(2);
        if (gLightSpritePriority == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

#undef LIGHT_SMALL_SIZE
#undef LIGHT_LARGE_HEIGHT
#undef LIGHT_LEFT
