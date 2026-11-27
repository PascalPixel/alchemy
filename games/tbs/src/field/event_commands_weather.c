/* @file */
/* The weather of the field: the rain splashing round the leader and the */
/* lightning of the storm over Vale, the sparkles of light in Sol Sanctum, and
 */
/* rising motes of light that no scene starts. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* The frames of a raindrop's splash on the ground */
/* (Weather_RainSplashFrames, five halfwords each), played from the */
/* last down to the first. */
#define RAIN_SPLASH_FRAMES 16

/* mote.png's 16 by 16 pictures, four tiles each: two of each size, the */
/* motes shrinking as they rise and flickering between the two. */
#define MOTE_PICTURE_TILES 4
#define MOTE_LARGE		   0
#define MOTE_MEDIUM		   2
#define MOTE_SMALL		   4

/* Task of the rain: each drop splashes on the ground and starts again at a new
 */
/* spot within 128 pixels of the leader. */
void fdWeatherUpdateRain(void) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	WEATHER_STATE *state = WeatherStateWork;
	s32 cameraX = syMapGridScrollStateView(grid)[0];
	s32 cameraZ = syMapGridScrollStateView(grid)[1];
	WEATHER_PARTICLE *particle = state->particles;
	u32 i;

	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
		const s16 *frame;
		s32 x;
		s32 y;

		if(--particle->tmr == GROUND_PARTICLE_FREED)
			continue;
		/* BUG: freezing a timer of 16 indexes past the 16-frame table. */
		if(gmFlagGetStub(FLAG_WEATHER_HELD))
			particle->tmr++;
		frame = &Weather_RainSplashFrames[5 * particle->tmr];
		x = (particle->posX - cameraX) / FIXED_ONE + *frame++;
		y = (particle->posZ - particle->posY - cameraZ) / FIXED_ONE + *frame++;
		if((x >= -16 && x <= DISPLAY_WIDTH - 1) && y >= -32 &&
		   y <= DISPLAY_HEIGHT - 1) {
			particle->pri = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = state->tileBase + *(const u16 *)frame;
			frame++;
			particle->shape = *(const u8 *)frame;
			particle->size = *(const u8 *)(frame + 1);
			syOamAddSprAtDepth(particle, OAM_DEPTH_WINDOW);
		}
		if(particle->tmr == 0) {
			s32 *leader = grid->origin;

			/* A new spot up to 128 pixels either way of the leader. */
			x = leader[0] + (syRandom() << 8) - toFIXED(128);
			y = leader[2] + (syRandom() << 8) - toFIXED(128);
			particle->posX = x;
			particle->posZ = y;
			particle->posY = toFIXED(
				fdMapGridGetTerrainHeightStub(0, FIXEDtoINT(x), FIXEDtoINT(y)));
			particle->tmr = RAIN_SPLASH_FRAMES;
		}
	}
}

/* Task for the storm: lightning flashes the BG colours three times, 5 frames */
/* apart. */
void fdWeatherUpdateLightning(void) {
	FIELD_BLEND_STATE *state = FieldBlendStateWork[0];
	PALETTE_FADE *target = PaletteFadeState;

	if(state->tmr < 0)
		return;
	if(gmFlagGetStub(FLAG_WEATHER_HELD))
		state->tmr = 128;
	switch(state->tmr--) {
	case 0:
		/* The burst ends: arm the next. */
		if(state->enabled != 0) {
			state->tmr = syRandom16Below(400) - syRandom16Below(100) + 150;
			if(state->loud != 0)
				sySndPlayStub(SE_THUNDER_LOUD);
			else
				sySndPlayStub(SE_THUNDER);
		}
		/* fall through */
	case 5:
	case 10:
		/* A frame after each flash: fade back to the dark colours over 12
		 * frames. */
		fdPaletteFadeSetTarget((s32)state, 1);
		syDMAXCopy(state->delta, target->delta,
				   DMA_COPY_WORDS(sizeof target->delta / 4), REG_DMA3);
		target->duration = 12;
		target->step = 0;
		break;
	case 1:
	case 6:
	case 11:
		/* The flash: the lit colours, in one frame. */
		fdPaletteFadeSetTarget((s32)state->to, 1);
		target->duration = 1;
		target->step = 0;
		break;
	}
}

/* Starts the rain (fdWeatherUpdateRain) with its sound, SE_AMBIENT_WATER: */
/* loads the splash tiles and puts 32 drops under the leader with staggered */
/* timers, drawn semi-transparent. */
void fdWeatherStartRain(void) {
	WEATHER_STATE *state;
	s32 *leader;
	WEATHER_PARTICLE *particle;
	/* words is the decode buffer, then each particle's attribute cursor. */
	u32 *words;
	u32 i;
	u32 fill = 0;

	state = syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, sizeof(WEATHER_STATE));
	leader = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
	scQueueMusic(SE_AMBIENT_WATER);
	particle = state->particles;
	btDmaFill32(fill, state, sizeof(WEATHER_STATE));
	words = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	fdDecompressByteLz(Weather_ParticleTiles, words);
	state->entry = syVramCacheFindFreeEntry();
	state->tileBase = syVramCacheLoad(state->entry, 768, words);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
		FIXED x;
		FIXED z;

		words = (u32 *)particle;
		*words++ = 0;
		*words++ = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		*words = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
		x = leader[0];
		z = leader[2];
		particle->posX = x;
		particle->posZ = z;
		particle->posY = toFIXED(
			fdMapGridGetTerrainHeightStub(0, FIXEDtoINT(x), FIXEDtoINT(z)));
		particle->tmr = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
		particle++;
	}
	scIoWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	scIoWrite16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
	scIoWrite16(0, &REG_BLDY);
	sySetEvent(fdWeatherUpdateRain, TASK_PASS_FRAME);
}

/* A particle's first word, seen as a value or a link. */
typedef union PARTICLE_LINK_WORD {
	u32 value;
	void *link;
} PARTICLE_LINK_WORD;

/* Task of the motes of light: each rises and shrinks for 120 frames, then */
/* starts again within 128 pixels of the leader, eight new ones a frame at */
/* most. */
void fdMotesUpdate(void) {
	WEATHER_STATE *state = WeatherStateWork;
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	u32 spawned = 0;
	u32 i;
	WEATHER_PARTICLE *mote = state->particles;

	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++, mote++) {
		s32 x;
		s32 y;

		if(--mote->tmr != GROUND_PARTICLE_FREED) {
			s32 cameraX = syMapGridScrollStateView(grid)[0];
			s32 cameraZ = syMapGridScrollStateView(grid)[1];
			u16 age = mote->tmr;
			u32 firstJitterDraw;
			u32 secondJitterDraw;

			if(gmFlagGetStub(FLAG_WEATHER_HELD)) {
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
			y = FIXEDtoINT(mote->posZ - mote->posY - cameraZ) - age;
			if((u32)(x + 16) <= 255 && y >= -32 && y <= DISPLAY_HEIGHT - 1) {
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
				mote->shape = 0;
				mote->size = 1;
				syOamAddSprAtDepth(mote, OAM_DEPTH_WINDOW);
			}
			else {
				mote->tmr = 0;
			}
		}
		if(spawned < 8 && mote->tmr == 0) {
			s32 *leader = grid->origin;

			x = leader[0] + (syRandom() << 8) - toFIXED(128);
			y = leader[2] + (syRandom() << 8) - toFIXED(128);
			mote->posZ = y;
			mote->posX = x;
			mote->posY = toFIXED(
				fdMapGridGetTerrainHeightStub(0, FIXEDtoINT(x), FIXEDtoINT(y)));
			mote->tmr = 120;
			mote->delay = 0;
			spawned++;
		}
	}
}

/* Starts the floating motes (fdMotesUpdate): loads their tiles and puts */
/* 32 of them under the leader with staggered timers, drawn semi-transparent. */
void fdMotesStart(void) {
	WEATHER_STATE *state;
	WEATHER_PARTICLE *mote;
	u8 *tiles;
	u32 i;
	u32 size;

	size = sizeof(WEATHER_STATE);
	state = syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, size);
	mote = state->particles;
	/* size, done with once the state is allocated, carries the fill's 0. */
	size = 0;
	btDmaFill32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	fdDecompressByteLz(FieldEffect_MoteTiles, tiles);
	state->entry = syVramCacheFindFreeEntry();
	state->tileBase = syVramCacheLoad(state->entry, 768, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
		s32 *pos = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
		/* p walks the sprite's attribute words, then holds the x, as size
		 * carries */
		/* the fill's 0 above. */
		u32 p = (u32)mote;
		FIXED z;
		*(s32 *)p = 0;
		p += sizeof(s32);
		*(s32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		p += sizeof(s32);
		*(s32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
		p = pos[0];
		z = pos[2];
		mote->posX = (s32)p;
		mote->posZ = z;
		mote->posY = toFIXED(fdMapGridGetTerrainHeightStub(
			0, FIXEDtoINT((s32)p), FIXEDtoINT(z)));
		/* Staggered as the raindrops are. */
		mote->tmr = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
		mote++;
	}
	scIoWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	scIoWrite16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
	scIoWrite16(0, &REG_BLDY);
	sySetEvent(fdMotesUpdate, TASK_PASS_FRAME);
}

/* Task of the sparkles: now and then a burst of up to four twinkles on one */
/* spot within 128 pixels of the leader. */
void fdEffectUpdateSparkles(void) {
	WEATHER_STATE *state;
	u32 spawned;
	u32 i;
	MAP_GRID_SCROLL_STATE *grid;
	s32 *camera;
	FIXED spawnX;
	FIXED spawnZ;
	WEATHER_PARTICLE *sparkle;
	const u16 *frame;
	s32 *leader;
	s32 cameraX;
	s32 cameraZ;
	s32 sx;
	s32 sy;
	s32 delay;
	s32 x;
	u32 a;
	u32 b;
	u32 frames;

	state = WeatherStateWork;
	spawned = 0;
	grid = MapGridState[0];
	camera = grid->view;
	spawnX = 0;
	spawnZ = 0;
	i = 0;
	sparkle = state->particles;
	delay = 0;
	sx = 0;
	sy = 0;
	do {
		if(sparkle->tmr != 0) {
			cameraX = camera[0];
			cameraZ = camera[1];
			if(gmFlagGetStub(FLAG_WEATHER_HELD) != 0)
				sparkle->tmr++;
			frame =
				(const u16 *)&FieldEffect_SparklePictures[sparkle->tmr >> 1];
			a = syRandom();
			b = syRandom();
			x = FIXEDtoINT(sparkle->posX - cameraX) +
				(((a & 1) + (b & 1)) >> 1);
			sx = x - 1;
			sy = (sparkle->posZ - sparkle->posY - cameraZ) / FIXED_ONE +
				 *(const s16 *)frame;
			frame++;
			if((u32)(x + 15) <= 255 && sy >= -32 && sy <= DISPLAY_HEIGHT - 1) {
				sparkle->tile = state->tileBase + *frame;
				sparkle->x = sx;
				sparkle->shape = 0;
				sparkle->size = 1;
#ifdef UBFIX
				frames = FrameCount;
#else
				/* UB: this drops volatile and reads a u32 as unsigned long.
				 * UBFIX reads */
				/* the declared type. */
				frames = *(unsigned long *)&FrameCount;
#endif
				sparkle->y = sy;
				sparkle->affineIdx = ((frames >> 1) & 1) << 3;
				syOamAddSprAtDepth(sparkle, OAM_DEPTH_WINDOW);
			}
			sparkle->tmr--;
		}
		if(spawned <= 3 && sparkle->tmr == 0 && state->stopped == 0) {
			if(delay != 0) {
				/* The rest of a burst, on the first sparkle's spot. */
				sparkle->posX = spawnX;
				sparkle->posZ = spawnZ;
				sparkle->posY = toFIXED(fdMapGridGetTerrainHeightStub(
					0, FIXEDtoINT(sx), FIXEDtoINT(sy)));
				sparkle->tmr = 62 - delay;
				sparkle->delay = 0;
				spawned++;
				delay += 4;
			}
			else if((syRandom() & 255) == 0) {
				/* A burst starts at a random spot. */
				leader = grid->origin;
				spawnX = leader[0] + (syRandom() << 8) - toFIXED(128);
				spawnZ = leader[2] + (syRandom() << 8) - toFIXED(128);
				sparkle->posX = spawnX;
				sparkle->posZ = spawnZ;
				sparkle->posY = toFIXED(fdMapGridGetTerrainHeightStub(
					0, FIXEDtoINT(sx), FIXEDtoINT(sy)));
				sparkle->tmr = 30;
				sparkle->delay = delay;
				spawned++;
				delay = 4;
			}
		}
		i++;
		sparkle++;
	} while(i < WEATHER_PARTICLE_COUNT);
}

/* Starts the sparkles (fdEffectUpdateSparkles): loads their tiles and */
/* clears the 32 sparkles. */
void fdEffectInitSparkles(void) {
	u32 size = sizeof(WEATHER_STATE);
	WEATHER_STATE *state = syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, size);
	WEATHER_PARTICLE *sparkle = state->particles;
	u8 *tiles;
	u32 i;

	/* size, done with once the state is allocated, carries the fill's 0. */
	size = 0;
	btDmaFill32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	fdDecompressByteLz(FieldEffect_SparkleTiles, tiles);
	state->entry = syVramCacheFindFreeEntry();
	state->tileBase = syVramCacheLoad(state->entry, 512, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	i = 0;
	do {
		MAP_GRID_SCROLL_STATE *view = MapGridState[0];
		s32 *leader;
		/* attr walks the sprite's attribute words, then holds the x. */
		u32 attr;
		s32 z;

		attr = (u32)sparkle;
		*(u32 *)attr = 0;
		attr += sizeof(PARTICLE_LINK_WORD);
		leader = view->origin;
		*(u32 *)attr = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		attr += sizeof(PARTICLE_LINK_WORD);
		*(u32 *)attr = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
		attr = leader[0];
		z = leader[2];
		sparkle->posX = 0;
		sparkle->posZ = 0;
		sparkle->posY = toFIXED(fdMapGridGetTerrainHeightStub(
			0, FIXEDtoINT((s32)attr), FIXEDtoINT(z)));
		/* Staggered as the raindrops are. */
		sparkle->tmr = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
		i++;
		sparkle++;
	} while(i < WEATHER_PARTICLE_COUNT);
	sySetEvent(fdEffectUpdateSparkles, TASK_PASS_FRAME);
}

/* Starts the storm's lightning (fdWeatherUpdateLightning): builds the dark */
/* (PALETTE_TINT_DIM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
 */
/* fade, and arms the first burst to end in 600 frames, repeating. */
void fdWeatherStartLightning(void) {
	FIELD_BLEND_STATE *state;
	PALETTE_FADE *buffers;

	state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = PaletteFadeState;
	btDmaFill32(0, state, sizeof(FIELD_BLEND_STATE));
	fdPaletteFadeBuild(PALETTE_TINT_DIM, buffers, state, 1);
	fdPaletteFadeBuild(PALETTE_TINT_PALE, buffers, state->to, 1);
	fdPaletteFadeCalcSteps(state->to, state->from, state->delta, 12);
	fdPaletteFadeBuild((s32)state, 0, buffers->tgt, 1);
	state->tmr = 600;
	state->enabled = 1;
	sySetEvent(fdWeatherUpdateLightning, TASK_PASS_FRAME);
}

/* Stops the lightning: no flash for 32767 frames, and none after. */
void fdWeatherStopLightning(void) {
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 32767;
	state->enabled = 0;
}

/* One last burst of lightning, its first flash two frames from now (timer 12;
 */
/* fdWeatherUpdateLightning flashes at 11), then no more. */
void fdWeatherLastLightning(void) {
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 12;
	state->enabled = 0;
}

/* Restarts the lightning: the next burst ends in 128 frames, its first flash */
/* 118 frames from now, and bursts repeat. */
void fdWeatherResumeLightning(void) {
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 128;
	state->enabled = 1;
}

/* Flashes the palette with the storm's task: one burst of three flashes of */
/* palette to, each fading back to palette from (fdPaletteFadeBuild modes) on */
/* the BG colours, ending 120 frames from now, and no more after. */
void fdWeatherStartLightningFlash(s32 from, s32 to) {
	FIELD_BLEND_STATE *state;
	PALETTE_FADE *buffers;
	s16 *target;

	state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = PaletteFadeState;
	btDmaFill32(0, state, sizeof(FIELD_BLEND_STATE));
	fdPaletteFadeBuild(from, buffers, state, 1);
	target = state->to;
	fdPaletteFadeBuild(to, buffers, target, 1);
	fdPaletteFadeCalcSteps(target, state->from, state->delta, 12);
	fdPaletteFadeBuild((s32)state, 0, buffers->tgt, 1);
	state->tmr = 120;
	state->enabled = 0;
	sySetEvent(fdWeatherUpdateLightning, TASK_PASS_FRAME);
}
