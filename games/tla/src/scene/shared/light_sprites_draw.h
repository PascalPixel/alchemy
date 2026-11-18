/// The glow sprites: 16 pixels square, or 32 by 64 with gLightSpritesLarge,
/// in 256 colours, each frame of the glow 8 tiles on from the last. Both stand
/// on their actor, LIGHT_LEFT pixels to its left.
#define LIGHT_SMALL_SIZE 16
#define LIGHT_LARGE_HEIGHT 64
#define LIGHT_LEFT 8
/// The bits of a sprite's x and y in its OAM attributes.
#define OBJ_X_MASK 0x1ff
#define OBJ_Y_MASK 0xff

/// Draws a glow sprite over each listed actor.
void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
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
      s32 big, ox;
      FIXED y, z;
      s32 oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x.fixed - viewX;
      y = actor->y.fixed - viewY + toFIXED(8);
      z = actor->z.fixed - viewZ - viewY;
      oy = z - y;
      depth = FIXEDtoINT(y + z) + 58;
      big = gLightSpritesLarge;
      if (big == 0) {
        s32 x, sy;
        frame = actor->timer;
        x = FIXEDtoINT(ox);
        sy = FIXEDtoINT(oy);
        ox = x - LIGHT_LEFT;
        oy = sy - LIGHT_SMALL_SIZE;
        if ((u32)(x + 7) > 334 || oy <= -LIGHT_SMALL_SIZE || oy > 239)
          continue;
        ox &= OBJ_X_MASK;
        oy &= OBJ_Y_MASK;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | OBJ_X(ox) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
        *word = (tile + frame * 8) | OBJ_PRIORITY(2);
        if (gLightSpritePriority == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = actor->timer;
        x = FIXEDtoINT(ox);
        sy = FIXEDtoINT(oy);
        ox = x - LIGHT_LEFT;
        oy = sy - LIGHT_LARGE_HEIGHT;
        if ((u32)(x + 23) > 350 || oy <= -LIGHT_LARGE_HEIGHT || oy > 175)
          continue;
        ox &= OBJ_X_MASK;
        oy &= OBJ_Y_MASK;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | OBJ_X(ox) | OBJ_SIZE_32X64 | OBJ_256_COLORS;
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
#undef OBJ_X_MASK
#undef OBJ_Y_MASK
