/* Weather drawn in sprites on the field, and the storm's lightning: floating
   motes, rain, rising sparkles, and the flashes of the palettes. Part of
   script_commands.c. */

/* The ticks the first motes and sparkles are spread over as they start. */
#define FIRST_SPAWN_TICKS 16

#define syDecompressByteLZ ((void (*)(const void *, void *))syDecompressByteLZ)

/* Unused: nothing calls fdMotesStartStub. Starts the 64 floating motes
   (fdMotesUpdate): loads their tiles and puts each under the leader with
   staggered timers, drawn semi-transparent. */
void fdMotesStart(void)
{
	WEATHER_STATE	 *state;
	WEATHER_PARTICLE *mote;
	Uint8			 *tiles;
	Uint32			  i;
	Uint32			  size;
	HEAP_SLOTS		 *heap;

	size = sizeof(WEATHER_STATE);
	state = syHeapSlotAllocEWRAM(HEAP_SLOT_WEATHER, size);
	mote = state->particles;
	size = 0;
	DMA_FILL32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLZ(FieldEffectMoteTiles, tiles);
	state->entry = syVRAMCacheFindFreeEntry();
	state->tileBase =
		syVRAMCacheLoad(state->entry, 6 * WEATHER_PICTURE_BYTES, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	i = 0;
	heap = RamHeapSlots;
	for(; i < WEATHER_PARTICLE_COUNT; i++) {
		MAP_STATE *view = heap->mapGrid;
		Sint32	  *pos;
		/* p walks the sprite's attribute words, then holds the x, as size
		   carries the fill's 0 above. */
		Uint32 p;
		Sint32 z;

		p = (Uint32)mote;
		*(Sint32 *)p = 0;
		p += sizeof(Sint32);
		pos = view->origin;
		*(Sint32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		p += sizeof(Sint32);
		*(Sint32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		p = pos[0];
		z = pos[2];
		mote->posX = (Sint32)p;
		mote->posZ = z;
		mote->posY = fdMapGridGetTerrainHeightStub(0, (Sint32)p, z);
		mote->tmr = (i & (FIRST_SPAWN_TICKS - 1)) + 1;
		mote++;
	}
	syIOWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	syIOWrite16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
	syIOWrite16(0, &REG_BLDY);
	sySetEvent(fdMotesUpdate, EVENT_PASS_TICK);
}

#undef syDecompressByteLZ

/* Starts fdWeatherUpdateRain's raindrops: allocates and clears
   HEAP_SLOT_WEATHER, decodes the streak tiles into VRAM, sets each of the 64
   drops' sprite words and places it on the ground at the map's focus with its
   first fall staggered, turns on alpha blending of all layers at 16/16,
   schedules the update and, when asked, plays SE_KIRIN (fdQueueMusic). */
void fdWeatherStartRain(Sint32 withSnd)
{
	WEATHER_STATE	 *state;
	WEATHER_PARTICLE *particle;
	Uint8			 *tiles;
	Uint32			  i;
	Uint32			  size;
	HEAP_SLOTS		 *heap;

	size = sizeof(WEATHER_STATE);
	state = syHeapSlotAllocEWRAM(HEAP_SLOT_WEATHER, size);
	particle = state->particles;
	size = 0;
	DMA_FILL32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLZ(FieldEffectStreakTiles, tiles);
	state->entry = syVRAMCacheFindFreeEntry();
	/* As many bytes as the motes' six pictures, though streak.png has two. */
	state->tileBase =
		syVRAMCacheLoad(state->entry, 6 * WEATHER_PICTURE_BYTES, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	i = 0;
	heap = RamHeapSlots;
	for(; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
		MAP_STATE *view = heap->mapGrid;
		Sint32	  *focus;
		/* p walks the sprite's attribute words, then holds the x, as size
		   carries the fill's 0 above. */
		Uint32 p;
		Sint32 z;

		p = (Uint32)particle;
		*(Sint32 *)p = 0;
		p += sizeof(Sint32);
		focus = view->origin;
		*(Sint32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		p += sizeof(Sint32);
		*(Sint32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		p = focus[0];
		z = focus[2];
		particle->posX = (Sint32)p;
		particle->posZ = z;
		particle->posY = fdMapGridGetTerrainHeightStub(0, (Sint32)p, z);
		/* Timers 1 to 4 and 9 to 12, by the drop's index. */
		particle->tmr = (i & 0xb) + 1;
	}
	syIOWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	syIOWrite16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
	syIOWrite16(0, &REG_BLDY);
	sySetEvent(fdWeatherUpdateRain, EVENT_PASS_TICK);
	if(withSnd != 0)
		fdQueueMusic(SE_KIRIN);
}

/* Stops fdWeatherStartRain's particles. Unused: nothing calls
   fdWeatherStopRainStub. */
void fdWeatherStopRain(void)
{
	syCloseEvent(fdWeatherUpdateRain);
}

/* The sparkles each tick: now and then a burst of four rises from a random
   spot within 128 pixels of the map's focus, each a tick of
   FieldEffectSparklePictures every two ticks and started 4 ticks after the
   one before; FLAG_WEATHER_HELD holds them where they are. */
void fdEffectUpdateSparkles(void)
{
	WEATHER_STATE	 *state;
	Uint32			  spawned;
	Uint32			  i;
	MAP_STATE		 *map;
	Sint32			 *camera;
	Sint32			  spawnX;
	Sint32			  spawnZ;
	WEATHER_PARTICLE *sparkle;
	const Uint16	 *frm;
	Sint32			 *leader;
	Sint32			  cameraX;
	Sint32			  cameraZ;
	Sint32			  screenX;
	Sint32			  screenY;
	Sint32			  delay;
	Sint32			  x;
	Uint32			  a;
	Uint32			  b;
	Uint32			  ticks;

	state = RamHeapSlots->weather;
	spawned = 0;
	map = RamHeapSlots->mapGrid;
	camera = map->view;
	spawnX = 0;
	spawnZ = 0;
	i = 0;
	sparkle = state->particles;
	delay = 0;
	screenX = 0;
	screenY = 0;
	do {
		if(sparkle->tmr != 0) {
			cameraX = camera[0];
			cameraZ = camera[1];
			if(syGameFlagGet(FLAG_WEATHER_HELD) != 0)
				sparkle->tmr++;
			frm =
				(const Uint16 *)&FieldEffectSparklePictures[sparkle->tmr >> 1];
			a = syRandom();
			b = syRandom();
			x = FIXEDtoINT(sparkle->posX - cameraX) +
				(((a & 1) + (b & 1)) >> 1);
			screenX = x - 1;
			screenY = (sparkle->posZ - sparkle->posY - cameraZ) / toFIXED(1) +
				*(const Sint16 *)frm;
			frm++;
			if(x >= -15 && x <= DISPLAY_WIDTH && screenY >= -32 &&
				screenY < DISPLAY_HEIGHT) {
				sparkle->tile = state->tileBase + *frm;
				sparkle->x = screenX;
				sparkle->shape = OBJ_SHAPE_SQUARE;
				sparkle->size = OAM_SIZE_16X16;
#if FIX_TYPE_PUNNING
				ticks = TickCount;
#else
				/* UB: this reads a Uint32 as unsigned long; UBFIX reads the
				   declared type. */
				ticks = *(unsigned long *)&TickCount;
#endif
				sparkle->y = screenY;
				sparkle->affineIdx = ((ticks >> 1) & 1) << 3;
				syOAMAddSprAtDepthClamped(sparkle, OAM_DEPTH_WINDOW);
			}
			sparkle->tmr--;
		}
		if(spawned <= 3 && sparkle->tmr == 0 && state->stopped == 0) {
			if(delay != 0) {
				/* The rest of a burst, on the first sparkle's spot. */
				sparkle->posX = spawnX;
				sparkle->posZ = spawnZ;
				/* BUG: the ground's height is read at the last sparkle drawn's
				   screen place taken as a 16.16 position, near the map's top
				   left corner, rather than under the burst; the same below. */
#if FIX_SPARKLE_GROUND_HEIGHT
				sparkle->posY =
					fdMapGridGetTerrainHeightStub(0, spawnX, spawnZ);
#else
				sparkle->posY =
					fdMapGridGetTerrainHeightStub(0, screenX, screenY);
#endif
				sparkle->tmr = 62 - delay;
				sparkle->delay = 0;
				spawned++;
				delay += 4;
			} else if((syRandom() & 255) == 0) {
				/* A burst starts at a random spot. */
				leader = map->origin;
				spawnX = leader[0] + (syRandom() << 8) - toFIXED(128);
				spawnZ = leader[2] + (syRandom() << 8) - toFIXED(128);
				sparkle->posX = spawnX;
				sparkle->posZ = spawnZ;
#if FIX_SPARKLE_GROUND_HEIGHT
				sparkle->posY =
					fdMapGridGetTerrainHeightStub(0, spawnX, spawnZ);
#else
				sparkle->posY =
					fdMapGridGetTerrainHeightStub(0, screenX, screenY);
#endif
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

#define syDecompressByteLZ ((void (*)(const void *, void *))syDecompressByteLZ)

/* Unused: nothing calls fdEffectInitSparklesStub; Aqua Rock and the Apojii
   Islands keep their own copy (shared/ground_particles_start.h). Starts the
   sparkles (fdEffectUpdateSparkles): loads their tiles and lays every sparkle
   at the map's top left corner with a staggered timer. */
void fdEffectInitSparkles(void)
{
	Uint32			  size = sizeof(WEATHER_STATE);
	WEATHER_STATE	 *state = syHeapSlotAllocEWRAM(HEAP_SLOT_WEATHER, size);
	WEATHER_PARTICLE *p = state->particles;
	Uint8			 *buf;
	Uint32			  i;
	Sint32			  clear;
	volatile Uint32	  zero;
	HEAP_SLOTS		 *heap;

	size = 0;
	DMA_FILL32_CELL(zero, size, state, sizeof(WEATHER_STATE));
	buf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLZ(FieldEffectSparkleTiles, buf);
	state->entry = syVRAMCacheFindFreeEntry();
	state->tileBase =
		syVRAMCacheLoad(state->entry, 4 * WEATHER_PICTURE_BYTES, buf);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);

	i = 0;
	heap = RamHeapSlots;
	clear = 0;
	do {
		MAP_STATE *view = heap->mapGrid;
		Sint32	  *leader;
		/* attr walks the sprite's attribute words, then holds the x. */
		Uint32 attr;
		Sint32 z;

		attr = (Uint32)p;
		*(Uint32 *)attr = clear;
		attr += sizeof(Uint32);
		leader = view->origin;
		*(Uint32 *)attr = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		attr += sizeof(Uint32);
		*(Uint32 *)attr = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		attr = leader[0];
		z = leader[2];
		p->posX = clear;
		p->posZ = clear;
		p->posY = fdMapGridGetTerrainHeightStub(0, attr, z);
		p->tmr = (i & (FIRST_SPAWN_TICKS - 1)) + 1;
		i++;
		p++;
	} while(i < WEATHER_PARTICLE_COUNT);
	sySetEvent(fdEffectUpdateSparkles, EVENT_PASS_TICK);
}

/* Starts the storm's lightning (fdWeatherUpdateLightning): builds the dark
   (PALETTE_TINT_WARM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
   fade, and arms the first burst to end in 600 ticks, repeating. */
void fdWeatherStartLightning(void)
{
	FIELD_BLEND_STATE *state;
	PALETTE_FADE	  *buffers;
	volatile Uint32	   zero;

	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = RamHeapSlots->palFade;
	DMA_FILL32_CELL(zero, 0, state, sizeof(*state));
	fdPaletteFadeBuild(
		PALETTE_TINT_WARM, (Uint16 *)buffers, (Uint16 *)state, 1);
	fdPaletteFadeBuild(
		PALETTE_TINT_PALE, (Uint16 *)buffers, (Uint16 *)state->to, 1);
	fdPaletteFadeCalcSteps(state->to, state->from, state->delta, 12);
	fdPaletteFadeBuild((Sint32)state, 0, buffers->tgt, 1);
	state->tmr = 600;
	state->enabled = 1;
	sySetEvent(fdWeatherUpdateLightning, EVENT_PASS_TICK);
}

#include "field/weather_lightning.c"

/* Flashes the palette with the storm's event: one burst of three flashes of
   palette to, each fading back to palette from (fdPaletteFadeBuild modes) on
   the BG colours, ending 120 ticks from now, and no more after. */
void fdWeatherStartLightningFlash(Sint32 from, Sint32 to)
{
	FIELD_BLEND_STATE *state;
	PALETTE_FADE	  *buffers;
	volatile Uint32	   zero;
	Sint16			  *tgt;

	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = RamHeapSlots->palFade;
	DMA_FILL32_CELL(zero, 0, state, sizeof(*state));
	fdPaletteFadeBuild(from, (Uint16 *)buffers, (Uint16 *)state, 1);
	tgt = state->to;
	fdPaletteFadeBuild(to, (Uint16 *)buffers, (Uint16 *)tgt, 1);
	fdPaletteFadeCalcSteps(tgt, state->from, state->delta, 12);
	fdPaletteFadeBuild((Sint32)state, 0, buffers->tgt, 1);
	state->tmr = 120;
	state->enabled = 0;
	sySetEvent(fdWeatherUpdateLightning, EVENT_PASS_TICK);
}

#undef FIRST_SPAWN_TICKS
