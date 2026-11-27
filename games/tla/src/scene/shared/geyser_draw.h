/* Draws each geyser's column as sprites, a piece every 16 pixels of its height
 */
/* and a bobbing top, at most GEYSER_SPRITES of them. */
void btGeyserDraw(void) {
	GEYSER_WORK *state = GEYSER_STATE;
	GEYSER *entry = state->entries;
	s32 i;
	u32 tile = VramCacheEntries[state->entry].ofs >> 5;
	s32 frame;
	u32 *oam = state->block;
	s32 count = 0;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 viewX, viewZ, originY;
	ACTOR *actor;
	s32 pieceY, x;
	FIXED y;
	s32 screenY;
	FIXED z;
	s32 depth;
	u32 *p;

	viewX = syMapStateView(map)[0];
	viewX &= FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	originY = map->origin[1];
	for(i = 0; i < GEYSER_ENTRIES; i++, entry++) {
		if(entry->id == 0 || entry->height == 0)
			continue;
		actor = fdActorGetById(entry->id);
		frame = (FrameCount % 12) / 3 * 8 + 32;
		for(pieceY = 0; pieceY < entry->height; pieceY += 16) {
			if(count >= GEYSER_SPRITES)
				continue;
			x = actor->x.fixed - viewX;
			y = actor->y.fixed + toFIXED(pieceY) - originY;
			z = actor->z.fixed - viewZ - originY;
			screenY = z - y;
			x = FIXEDtoINT(x) - 8;
			screenY = FIXEDtoINT(screenY) - 16;
			depth = FIXEDtoINT(y + z) + SCENE_SPRITE_DEPTH;
			if((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
				continue;
			x &= OBJ_X_MASK;
			screenY &= OBJ_Y_MASK;
			p = oam;
			*p++ = 0;
			*p++ = screenY | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
			*p = (tile + frame) | OBJ_PRIORITY(2);
			syOamAddSprAtDepthClamped(oam, depth);
			oam += 3;
			count++;
		}
		if(count >= GEYSER_SPRITES)
			continue;
		x = actor->x.fixed - viewX;
		y = actor->y.fixed + toFIXED(entry->height) - originY;
		z = actor->z.fixed - viewZ - originY;
		screenY = z - y;
		x = FIXEDtoINT(x) - 8;
		screenY =
			FIXEDtoINT(screenY + syMathMulFX(-toFIXED(2), sySin(entry->phase)) -
					   toFIXED(2)) -
			4;
		depth = FIXEDtoINT(y + z) + SCENE_SPRITE_DEPTH;
		if((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
			continue;
		x &= OBJ_X_MASK;
		screenY &= OBJ_Y_MASK;
		p = oam;
		*p++ = 0;
		*p++ = screenY | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		*p = (tile + frame - 32) | OBJ_PRIORITY(2);
		syOamAddSprAtDepthClamped(oam, depth);
		oam += 3;
		count++;
	}
}
