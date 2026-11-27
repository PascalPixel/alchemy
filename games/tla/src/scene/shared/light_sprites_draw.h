/* The glow sprites: 16 pixels square, or 32 by 64 with LightSpritesLarge, in
   256 colours, each frame of the glow 8 tiles on from the last. Both stand on
   their actor, LIGHT_LEFT pixels to its left. */
#define LIGHT_SMALL_SIZE   16
#define LIGHT_LARGE_HEIGHT 64
#define LIGHT_LEFT		   8

/* Draws a glow sprite over each listed actor. */
void scLightSprsDraw(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   viewX, viewZ, tile, viewY;
	LIGHT_OAM *oam;
	Sint32	   i;
	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	tile = VramCacheEntries[LightSpriteTile].ofs >> 5;
	viewY = map->origin[1];
	oam = LightSpriteOams;

	for(i = 0; i < LightSpriteCount; i++) {
		ACTOR *actor = LightSpriteActors[i];
		if(actor != 0 && actor->x.fixed != 0) {
			Sint32	large, screenX;
			FIXED	y, z;
			Sint32	screenY, depth;
			Sint32	frame;
			Uint32 *word;
			screenX = actor->x.fixed - viewX;
			y = actor->y.fixed - viewY + toFIXED(8);
			z = actor->z.fixed - viewZ - viewY;
			screenY = z - y;
			depth = FIXEDtoINT(y + z) + SCENE_SPRITE_DEPTH;
			large = LightSpritesLarge;
			if(large == 0) {
				Sint32 actorX, actorY;
				frame = actor->tmr;
				actorX = FIXEDtoINT(screenX);
				actorY = FIXEDtoINT(screenY);
				screenX = actorX - LIGHT_LEFT;
				screenY = actorY - LIGHT_SMALL_SIZE;
				if((Uint32)(actorX + 7) > 334 || screenY <= -LIGHT_SMALL_SIZE ||
					screenY > 239)
					continue;
				screenX &= OBJ_X_MASK;
				screenY &= OBJ_Y_MASK;
				word = (Uint32 *)oam;
				*word++ = 0;
				*word++ =
					screenY | OBJ_X(screenX) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
				*word = (tile + frame * 8) | OBJ_PRIORITY(2);
				if(LightSpritePriority == -1)
					oam->pri = fdActorGetSprPriority(actor);
				else
					oam->pri = LightSpritePriority;
				syOAMAddSprAtDepthClamped(oam, depth);
				oam++;
			} else {
				Sint32 actorX, actorY;
				frame = actor->tmr;
				actorX = FIXEDtoINT(screenX);
				actorY = FIXEDtoINT(screenY);
				screenX = actorX - LIGHT_LEFT;
				screenY = actorY - LIGHT_LARGE_HEIGHT;
				if((Uint32)(actorX + 23) > 350 ||
					screenY <= -LIGHT_LARGE_HEIGHT || screenY > 175)
					continue;
				screenX &= OBJ_X_MASK;
				screenY &= OBJ_Y_MASK;
				word = (Uint32 *)oam;
				*word++ = 0;
				*word++ =
					screenY | OBJ_X(screenX) | OBJ_SIZE_32X64 | OBJ_256_COLORS;
				*word = (tile + frame * 8) | OBJ_PRIORITY(2);
				if(LightSpritePriority == -1)
					oam->pri = fdActorGetSprPriority(actor);
				else
					oam->pri = LightSpritePriority;
				syOAMAddSprAtDepthClamped(oam, depth);
				oam++;
			}
		}
	}
}

#undef LIGHT_SMALL_SIZE
#undef LIGHT_LARGE_HEIGHT
#undef LIGHT_LEFT
