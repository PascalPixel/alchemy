/* The weather of the field: the rain splashing round the leader and the
   lightning of the storm over Vale, the sparkles of light in Sol Sanctum, and
   rising motes of light that no scene starts. Part of script_commands.c. */

#include "script_commands_internal.h"

/* The frames of a raindrop's splash on the ground (WeatherRainSplashFrames,
   five halfwords each), played from the last down to the first. */
#define RAIN_SPLASH_FRAMES 16

/* mote.png's 16 by 16 pictures, four tiles each: two of each size, the motes
   shrinking as they rise and flickering between the two. */
#define MOTE_PICTURE_TILES 4
#define MOTE_LARGE		   0
#define MOTE_MEDIUM		   2
#define MOTE_SMALL		   4

/* Event of the rain: each drop splashes on the ground and starts again at a new
   spot within 128 pixels of the leader. */
void fdWeatherUpdateRain(void)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	WEATHER_STATE		  *state = WeatherStateWork;
	Sint32				   cameraX = syMapGridScrollStateView(grid)[0];
	Sint32				   cameraZ = syMapGridScrollStateView(grid)[1];
	WEATHER_PARTICLE	  *particle = state->particles;
	Uint32				   i;

	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
		const Sint16 *frm;
		Sint32		  x;
		Sint32		  y;

		if(--particle->tmr == GROUND_PARTICLE_FREED)
			continue;
		/* BUG: freezing a timer of 16 indexes past the 16-tick table. */
		if(gmFlagGetStub(FLAG_WEATHER_HELD))
			particle->tmr++;
		frm = &WeatherRainSplashFrames[5 * particle->tmr];
		x = (particle->posX - cameraX) / toFIXED(1) + *frm++;
		y = (particle->posZ - particle->posY - cameraZ) / toFIXED(1) + *frm++;
		if((x >= -16 && x <= DISPLAY_WIDTH - 1) && y >= -32 &&
			y <= DISPLAY_HEIGHT - 1) {
			particle->pri = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = state->tileBase + *(const Uint16 *)frm;
			frm++;
			particle->shape = *(const Uint8 *)frm;
			particle->size = *(const Uint8 *)(frm + 1);
			syOAMAddSprAtDepth(particle, OAM_DEPTH_WINDOW);
		}
		if(particle->tmr == 0) {
			Sint32 *leader = grid->origin;

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

/* Event for the storm: lightning flashes the BG colours three times, 5 ticks
   apart. */
void fdWeatherUpdateLightning(void)
{
	FIELD_BLEND_STATE *state = FieldBlendStateWork[0];
	PALETTE_FADE	  *target = PaletteFadeState;

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
		/* A tick after each flash: fade back to the dark colours over 12
		   ticks. */
		fdPaletteFadeSetTarget((Sint32)state, 1);
		syDMAXCopy(state->delta, target->delta,
			DMA_COPY_WORDS(sizeof target->delta / 4), REG_DMA3);
		target->duration = 12;
		target->step = 0;
		break;
	case 1:
	case 6:
	case 11:
		/* The flash: the lit colours, in one tick. */
		fdPaletteFadeSetTarget((Sint32)state->to, 1);
		target->duration = 1;
		target->step = 0;
		break;
	}
}

/* Starts the rain (fdWeatherUpdateRain) with its sound, SE_AMBIENT_WATER: loads
   the splash tiles and puts 32 drops under the leader with staggered timers,
   drawn semi-transparent. */
void fdWeatherStartRain(void)
{
	WEATHER_STATE	 *state;
	Sint32			 *leader;
	WEATHER_PARTICLE *particle;
	/* words is the decode buffer, then each particle's attribute cursor. */
	Uint32 *words;
	Uint32	i;
	Uint32	fill = 0;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_WEATHER, sizeof(WEATHER_STATE));
	leader = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
	fdQueueMusic(SE_AMBIENT_WATER);
	particle = state->particles;
	DMA_FILL32(fill, state, sizeof(WEATHER_STATE));
	words = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLZ(WeatherParticleTiles, words);
	state->entry = syVRAMCacheFindFreeEntry();
	state->tileBase = syVRAMCacheLoad(state->entry, 768, words);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
		FIXED x;
		FIXED z;

		words = (Uint32 *)particle;
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
	IO_WRITE16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	IO_WRITE16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
	IO_WRITE16(0, &REG_BLDY);
	sySetEvent(fdWeatherUpdateRain, EVENT_PASS_TICK);
}

/* A particle's first word, seen as a value or a link. */
typedef union PARTICLE_LINK_WORD {
	Uint32 value;
	void  *link;
} PARTICLE_LINK_WORD;

/* Event of the motes of light: each rises and shrinks for 120 ticks, then
   starts again within 128 pixels of the leader, eight new ones a tick at
   most. */
void fdMotesUpdate(void)
{
	WEATHER_STATE		  *state = WeatherStateWork;
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	Uint32				   spawned = 0;
	Uint32				   i;
	WEATHER_PARTICLE	  *mote = state->particles;

	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++, mote++) {
		Sint32 x;
		Sint32 y;

		if(--mote->tmr != GROUND_PARTICLE_FREED) {
			Sint32 cameraX = syMapGridScrollStateView(grid)[0];
			Sint32 cameraZ = syMapGridScrollStateView(grid)[1];
			Uint16 age = mote->tmr;
			Uint32 firstJitterDraw;
			Uint32 secondJitterDraw;

			if(gmFlagGetStub(FLAG_WEATHER_HELD)) {
				mote->tmr++;
				mote->delay--;
			}
			/* x jitters by -1 or 0, and y is the ticks left in pixels above
			   the spot; a quarter of the delay, which builds up as it ages,
			   lifts it further. */
			firstJitterDraw = syRandom();
			secondJitterDraw = syRandom();
			x = FIXEDtoINT(mote->posX - cameraX) +
				(((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
			y = FIXEDtoINT(mote->posZ - mote->posY - cameraZ) - age;
			if((Uint32)(x + 16) <= 255 && y >= -32 && y <= DISPLAY_HEIGHT - 1) {
				if(mote->tmr < 60) {
					mote->tile =
						state->tileBase + MOTE_SMALL * MOTE_PICTURE_TILES;
					mote->delay += 3;
				} else if(mote->tmr < 90) {
					mote->tile =
						state->tileBase + MOTE_MEDIUM * MOTE_PICTURE_TILES;
					mote->delay++;
				} else {
					mote->tile =
						state->tileBase + MOTE_LARGE * MOTE_PICTURE_TILES;
				}
				if((TickCount >> 3) & 1)
					mote->tile += MOTE_PICTURE_TILES;
				mote->x = x;
				mote->y = y - (mote->delay >> 2);
				mote->shape = 0;
				mote->size = 1;
				syOAMAddSprAtDepth(mote, OAM_DEPTH_WINDOW);
			} else {
				mote->tmr = 0;
			}
		}
		if(spawned < 8 && mote->tmr == 0) {
			Sint32 *leader = grid->origin;

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

/* Starts the floating motes (fdMotesUpdate): loads their tiles and puts 32 of
   them under the leader with staggered timers, drawn semi-transparent. */
void fdMotesStart(void)
{
	WEATHER_STATE	 *state;
	WEATHER_PARTICLE *mote;
	Uint8			 *tiles;
	Uint32			  i;
	Uint32			  size;

	size = sizeof(WEATHER_STATE);
	state = syHeapSlotAllocEWRAM(HEAP_SLOT_WEATHER, size);
	mote = state->particles;
	/* size, done with once the state is allocated, carries the fill's 0. */
	size = 0;
	DMA_FILL32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLZ(FieldEffectMoteTiles, tiles);
	state->entry = syVRAMCacheFindFreeEntry();
	state->tileBase = syVRAMCacheLoad(state->entry, 768, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
		Sint32 *pos = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
		/* p walks the sprite's attribute words, then holds the x, as size
		   carries the fill's 0 above. */
		Uint32 p = (Uint32)mote;
		FIXED  z;
		*(Sint32 *)p = 0;
		p += sizeof(Sint32);
		*(Sint32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		p += sizeof(Sint32);
		*(Sint32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
		p = pos[0];
		z = pos[2];
		mote->posX = (Sint32)p;
		mote->posZ = z;
		mote->posY = toFIXED(fdMapGridGetTerrainHeightStub(
			0, FIXEDtoINT((Sint32)p), FIXEDtoINT(z)));
		/* Staggered as the raindrops are. */
		mote->tmr = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
		mote++;
	}
	IO_WRITE16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	IO_WRITE16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
	IO_WRITE16(0, &REG_BLDY);
	sySetEvent(fdMotesUpdate, EVENT_PASS_TICK);
}

/* Event of the sparkles: now and then a burst of up to four twinkles on one
   spot within 128 pixels of the leader. */
void fdEffectUpdateSparkles(void)
{
	WEATHER_STATE		  *state;
	Uint32				   spawned;
	Uint32				   i;
	MAP_GRID_SCROLL_STATE *grid;
	Sint32				  *camera;
	FIXED				   spawnX;
	FIXED				   spawnZ;
	WEATHER_PARTICLE	  *sparkle;
	const Uint16		  *frm;
	Sint32				  *leader;
	Sint32				   cameraX;
	Sint32				   cameraZ;
	Sint32				   sx;
	Sint32				   sy;
	Sint32				   delay;
	Sint32				   x;
	Uint32				   a;
	Uint32				   b;
	Uint32				   ticks;

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
			frm =
				(const Uint16 *)&FieldEffectSparklePictures[sparkle->tmr >> 1];
			a = syRandom();
			b = syRandom();
			x = FIXEDtoINT(sparkle->posX - cameraX) +
				(((a & 1) + (b & 1)) >> 1);
			sx = x - 1;
			sy = (sparkle->posZ - sparkle->posY - cameraZ) / toFIXED(1) +
				*(const Sint16 *)frm;
			frm++;
			if((Uint32)(x + 15) <= 255 && sy >= -32 &&
				sy <= DISPLAY_HEIGHT - 1) {
				sparkle->tile = state->tileBase + *frm;
				sparkle->x = sx;
				sparkle->shape = 0;
				sparkle->size = 1;
#if FIX_TYPE_PUNNING
				ticks = TickCount;
#else
				/* UB: this drops volatile and reads a Uint32 as unsigned long.
				   UBFIX reads the declared type. */
				ticks = *(unsigned long *)&TickCount;
#endif
				sparkle->y = sy;
				sparkle->affineIdx = ((ticks >> 1) & 1) << 3;
				syOAMAddSprAtDepth(sparkle, OAM_DEPTH_WINDOW);
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
			} else if((syRandom() & 255) == 0) {
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

/* Starts the sparkles (fdEffectUpdateSparkles): loads their tiles and clears
   the 32 sparkles. */
void fdEffectInitSparkles(void)
{
	Uint32			  size = sizeof(WEATHER_STATE);
	WEATHER_STATE	 *state = syHeapSlotAllocEWRAM(HEAP_SLOT_WEATHER, size);
	WEATHER_PARTICLE *sparkle = state->particles;
	Uint8			 *tiles;
	Uint32			  i;

	/* size, done with once the state is allocated, carries the fill's 0. */
	size = 0;
	DMA_FILL32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLZ(FieldEffectSparkleTiles, tiles);
	state->entry = syVRAMCacheFindFreeEntry();
	state->tileBase = syVRAMCacheLoad(state->entry, 512, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	i = 0;
	do {
		MAP_GRID_SCROLL_STATE *view = MapGridState[0];
		Sint32				  *leader;
		/* attr walks the sprite's attribute words, then holds the x. */
		Uint32 attr;
		Sint32 z;

		attr = (Uint32)sparkle;
		*(Uint32 *)attr = 0;
		attr += sizeof(PARTICLE_LINK_WORD);
		leader = view->origin;
		*(Uint32 *)attr = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		attr += sizeof(PARTICLE_LINK_WORD);
		*(Uint32 *)attr = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
		attr = leader[0];
		z = leader[2];
		sparkle->posX = 0;
		sparkle->posZ = 0;
		sparkle->posY = toFIXED(fdMapGridGetTerrainHeightStub(
			0, FIXEDtoINT((Sint32)attr), FIXEDtoINT(z)));
		/* Staggered as the raindrops are. */
		sparkle->tmr = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
		i++;
		sparkle++;
	} while(i < WEATHER_PARTICLE_COUNT);
	sySetEvent(fdEffectUpdateSparkles, EVENT_PASS_TICK);
}

/* Starts the storm's lightning (fdWeatherUpdateLightning): builds the dark
   (PALETTE_TINT_DIM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
   fade, and arms the first burst to end in 600 ticks, repeating. */
void fdWeatherStartLightning(void)
{
	FIELD_BLEND_STATE *state;
	PALETTE_FADE	  *buffers;

	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = PaletteFadeState;
	DMA_FILL32(0, state, sizeof(FIELD_BLEND_STATE));
	fdPaletteFadeBuild(PALETTE_TINT_DIM, buffers, state, 1);
	fdPaletteFadeBuild(PALETTE_TINT_PALE, buffers, state->to, 1);
	fdPaletteFadeCalcSteps(state->to, state->from, state->delta, 12);
	fdPaletteFadeBuild((Sint32)state, 0, buffers->tgt, 1);
	state->tmr = 600;
	state->enabled = 1;
	sySetEvent(fdWeatherUpdateLightning, EVENT_PASS_TICK);
}

/* Stops the lightning: no flash for 32767 ticks, and none after. */
void fdWeatherStopLightning(void)
{
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 32767;
	state->enabled = 0;
}

/* One last burst of lightning, its first flash two ticks from now (timer 12;
   fdWeatherUpdateLightning flashes at 11), then no more. */
void fdWeatherLastLightning(void)
{
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 12;
	state->enabled = 0;
}

/* Restarts the lightning: the next burst ends in 128 ticks, its first flash
   118 ticks from now, and bursts repeat. */
void fdWeatherResumeLightning(void)
{
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 128;
	state->enabled = 1;
}

/* Flashes the palette with the storm's event: one burst of three flashes of
   palette to, each fading back to palette from (fdPaletteFadeBuild modes) on
   the BG colours, ending 120 ticks from now, and no more after. */
void fdWeatherStartLightningFlash(Sint32 from, Sint32 to)
{
	FIELD_BLEND_STATE *state;
	PALETTE_FADE	  *buffers;
	Sint16			  *tgt;

	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = PaletteFadeState;
	DMA_FILL32(0, state, sizeof(FIELD_BLEND_STATE));
	fdPaletteFadeBuild(from, buffers, state, 1);
	tgt = state->to;
	fdPaletteFadeBuild(to, buffers, tgt, 1);
	fdPaletteFadeCalcSteps(tgt, state->from, state->delta, 12);
	fdPaletteFadeBuild((Sint32)state, 0, buffers->tgt, 1);
	state->tmr = 120;
	state->enabled = 0;
	sySetEvent(fdWeatherUpdateLightning, EVENT_PASS_TICK);
}
