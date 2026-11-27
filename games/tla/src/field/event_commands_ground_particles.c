/* @file */
/* The ground particles, the rising particles and the floating motes. */
/* Part of event_commands.c. */

/* mote.png's six pictures, four tiles each: two of each size, the motes */
/* shrinking as they rise and flickering between the two. */
#define MOTE_PICTURE_TILES SPRITE_TILES(16, 16)
#define MOTE_LARGE		   0
#define MOTE_MEDIUM		   2
#define MOTE_SMALL		   4

/* Unused: nothing calls fdGroundParticlesStartStub. The Broken Seal's rain, */
/* kept whole where this game has its own fdWeatherStartRain: 64 sprites placed
 */
/* at the leader's position on the terrain, their tiles decoded into VRAM, the
 */
/* blend set, and fdGroundParticlesUpdate scheduled to move them. */
void fdGroundParticlesStart(void) {
	WEATHER_STATE *state;
	s32 *leader;
	WEATHER_PARTICLE *particle;
	/* words is the decode buffer, then each particle's attribute cursor. */
	u32 *words;
	u32 i;
	u32 fill = 0;

	state = syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, sizeof(WEATHER_STATE));
	leader = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->origin;
	scQueueMusic(SE_AMBIENT_WATER);
	particle = state->particles;
	btDmaFill32(fill, state, sizeof(WEATHER_STATE));
	words = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLz(Weather_ParticleTiles, words);
	state->entry = syVramCacheFindFreeEntry();
	state->tileBase =
		syVramCacheLoad(state->entry, 6 * WEATHER_PICTURE_BYTES, words);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
		s32 x;
		s32 z;

		words = (u32 *)particle;
		*words++ = 0;
		*words++ = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		*words = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		x = leader[0];
		z = leader[2];
		particle->posX = x;
		particle->posZ = z;
		particle->posY =
			fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, z);
		particle->tmr = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
		particle++;
	}
	syIoWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	syIoWrite16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
	syIoWrite16(0, &REG_BLDY);
	sySetEvent(fdGroundParticlesUpdate, TASK_PASS_FRAME);
}

/* Task of fdRisingParticlesStart: updates the 64 rising particles; each starts
 */
/* on the ground at a random point of the strip for 120 frames, drifts right by
 */
/* a random amount each frame, rises with its age, and is drawn at priority 3,
 */
/* its tile stepping back by 8 at 90 and 60 frames left and flickering between
 */
/* two tiles every 8 frames. */
void fdRisingParticlesUpdate(void) {
	RISING_PARTICLE_STATE *state = Ram_HeapSlots->weather;
	RISING_PARTICLE *particle = state->particles;
	FIELD_COORDINATE screen[3];
	u32 i;
	s32 x;
	s32 y;

	for(i = 0; i < ARRAY_COUNT(state->particles); i++, particle++) {
		if(particle->tmr == 0) {
			x = syRandom() * 200 + FIXED_ONE * 288;
			y = FIXED_ONE * 80 - syRandom() * 20;
			particle->posX = x;
			particle->posZ = y;
			particle->posY =
				fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, y);
			particle->tmr = 120;
			particle->drift = 0;
		}
		screen[0].fixed = particle->posX + particle->drift;
		screen[1].fixed = particle->posY;
		screen[2].fixed = particle->posZ;
		fdCameraWorldToScr(screen);
		x = screen[0].part[0].integer;
		y = screen[2].part[0].integer - particle->tmr - 8;
		particle->drift += syRandom() + FIXED_ONE / 2;
		if(x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
			if(particle->tmr < 60)
				particle->spr.attributes.tile =
					state->tile + MOTE_SMALL * MOTE_PICTURE_TILES;
			else if(particle->tmr < 90)
				particle->spr.attributes.tile =
					state->tile + MOTE_MEDIUM * MOTE_PICTURE_TILES;
			else
				particle->spr.attributes.tile =
					state->tile + MOTE_LARGE * MOTE_PICTURE_TILES;
			particle->spr.attributes.tile =
				((FrameCount >> 3) & 1) * MOTE_PICTURE_TILES +
				particle->spr.attributes.tile;
			particle->spr.attributes.x = x;
			particle->spr.attributes.shape = OBJ_SHAPE_SQUARE;
			particle->spr.attributes.size = OAM_SIZE_16X16;
			particle->spr.attributes.pri = 3;
			particle->spr.attributes.objMode = OBJ_MODE_SEMI_TRANSPARENT;
			particle->spr.attributes.y = y;
			syOamAddSprAtDepthClamped(particle, OAM_DEPTH_WINDOW);
		}
		particle->tmr--;
	}
}

/* Starts the rising particles, as the Northern Reaches does: allocates and */
/* clears heap slot */
/* HEAP_SLOT_WEATHER, decodes the particle tiles into VRAM, sets each of the 64
 */
/* particles' sprite words and places it on the ground at a random point of the
 */
/* strip (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2 */
/* frames, and schedules the update. */
void fdRisingParticlesStart(void) {
	RISING_PARTICLE_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, sizeof(WEATHER_STATE));
	RISING_PARTICLE *particle = state->particles;
	volatile u32 fill;
	u8 *tiles;
	u32 i;
	FIXED z;

	/* FAKEMATCH(temporary): z, holding the fill's 0 before it is a particle's
	 * z; as 0 in the fill. */
	z = 0;
	btDmaFill32Cell(fill, z, state, sizeof(WEATHER_STATE));
	tiles = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLz(FieldEffect_MoteTiles, tiles);
	state->vramEntry = syVramCacheFindFreeEntry();
	state->tile =
		syVramCacheLoad(state->vramEntry, 6 * WEATHER_PICTURE_BYTES, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	for(i = 0; i < ARRAY_COUNT(state->particles); i++, particle++) {
		u32 *sprite = particle->spr.words;
		s32 x;

		*sprite++ = 0;
		*sprite++ = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		*sprite = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		x = syRandom() * 200 + toFIXED(288);
		z = toFIXED(80) - syRandom() * 20;
		particle->posX = x;
		particle->posZ = z;
		particle->posY =
			fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, z);
		particle->drift = 0;
		particle->tmr = i * 2;
	}
	sySetEvent(fdRisingParticlesUpdate, TASK_PASS_FRAME);
}

/* Task of fdMotesStart: moves and draws the 64 motes in HEAP_SLOT_WEATHER. */
/* Up to eight a frame start within 128 pixels of the point the view follows */
/* and live 120 frames, rising, shrinking at 90 and 60 frames left and */
/* flickering every 8 frames; FLAG_WEATHER_HELD holds them where they are. */
void fdMotesUpdate(void) {
	WEATHER_STATE *state = Ram_HeapSlots->weather;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	u32 spawned = 0;
	u32 i;
	WEATHER_PARTICLE *mote;

	for(i = 0, mote = state->particles; i < WEATHER_PARTICLE_COUNT;
		i++, mote++) {
		s32 x;
		s32 y;

		if(--mote->tmr != GROUND_PARTICLE_FREED) {
			s32 cameraX = syMapStateView(map)[0];
			s32 cameraZ = syMapStateView(map)[1];
			s16 age = mote->tmr;
			u32 firstJitterDraw;
			u32 secondJitterDraw;

			if(syGameFlagGet(FLAG_WEATHER_HELD)) {
				mote->tmr++;
				mote->delay--;
			}
			/* x jitters by -1 or 0, and y is the frames left in pixels above
			 * the */
			/* spot; a quarter of the delay, which builds up as it ages, lifts
			 * it */
			/* further. */
			firstJitterDraw = syRandom();
			secondJitterDraw = syRandom();
			x = FIXEDtoINT(mote->posX - cameraX) +
				(((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
			y = FIXEDtoINT(mote->posZ - mote->posY - cameraZ) - (u16)age;
			if(x >= -16 && x < DISPLAY_WIDTH && y >= -32 &&
			   y < DISPLAY_HEIGHT) {
				if(mote->tmr < 60) {
					mote->tile =
						state->tileBase + MOTE_SMALL * MOTE_PICTURE_TILES;
					mote->delay += 3;
				}
				else if(mote->tmr < 90) {
					mote->tile =
						state->tileBase + MOTE_MEDIUM * MOTE_PICTURE_TILES;
					mote->delay++;
				}
				else {
					mote->tile =
						state->tileBase + MOTE_LARGE * MOTE_PICTURE_TILES;
				}
				if((FrameCount >> 3) & 1)
					mote->tile += MOTE_PICTURE_TILES;
				mote->x = x;
				mote->y = y - (mote->delay >> 2);
				mote->shape = OBJ_SHAPE_SQUARE;
				mote->size = OAM_SIZE_16X16;
				syOamAddSprAtDepthClamped(mote, OAM_DEPTH_WINDOW);
			}
			else {
				mote->tmr = 0;
			}
		}
		if(spawned < 8 && mote->tmr == 0) {
			s32 *leader = map->origin;

			x = leader[0] + (syRandom() << 8) - toFIXED(128);
			y = leader[2] + (syRandom() << 8) - toFIXED(128);
			mote->posX = x;
			mote->posZ = y;
			mote->posY =
				fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, y);
			mote->tmr = 120;
			mote->delay = 0;
			spawned++;
		}
	}
}

#undef MOTE_PICTURE_TILES
#undef MOTE_LARGE
#undef MOTE_MEDIUM
#undef MOTE_SMALL
