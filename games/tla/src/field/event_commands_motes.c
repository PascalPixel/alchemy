/* @file */
/* Weather drawn in sprites on the field, and the storm's lightning: floating */
/* motes, rain, rising sparkles, and the flashes of the palettes. */
/* Part of event_commands.c. */

/* The frames the first motes and sparkles are spread over as they start. */
#define FIRST_SPAWN_FRAMES 16

#define syDecompressByteLz ((void (*)(const void *, void *))syDecompressByteLz)

/* Unused: nothing calls fdMotesStartStub. Starts the 64 floating motes */
/* (fdMotesUpdate): loads their tiles and puts each under the leader with */
/* staggered timers, drawn semi-transparent. */
void fdMotesStart(void) {
	WEATHER_STATE *state;
	WEATHER_PARTICLE *mote;
	u8 *tiles;
	u32 i;
	u32 size;
	HEAP_SLOTS *heap;

	size = sizeof(WEATHER_STATE);
	state = syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, size);
	mote = state->particles;
	size = 0;
	btDmaFill32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLz(FieldEffect_MoteTiles, tiles);
	state->entry = syVramCacheFindFreeEntry();
	state->tileBase =
		syVramCacheLoad(state->entry, 6 * WEATHER_PICTURE_BYTES, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	i = 0;
	heap = Ram_HeapSlots;
	for(; i < WEATHER_PARTICLE_COUNT; i++) {
		MAP_STATE *view = heap->mapGrid;
		s32 *pos;
		/* p walks the sprite's attribute words, then holds the x, as size
		 * carries */
		/* the fill's 0 above. */
		u32 p;
		s32 z;

		p = (u32)mote;
		*(s32 *)p = 0;
		p += sizeof(s32);
		pos = view->origin;
		*(s32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		p += sizeof(s32);
		*(s32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		p = pos[0];
		z = pos[2];
		mote->posX = (s32)p;
		mote->posZ = z;
		mote->posY = fdMapGridGetTerrainHeightStub(0, (s32)p, z);
		mote->tmr = (i & (FIRST_SPAWN_FRAMES - 1)) + 1;
		mote++;
	}
	syIoWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	syIoWrite16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
	syIoWrite16(0, &REG_BLDY);
	sySetEvent(fdMotesUpdate, TASK_PASS_FRAME);
}

#undef syDecompressByteLz

/* Starts fdWeatherUpdateRain's raindrops: allocates and clears */
/* HEAP_SLOT_WEATHER, decodes the streak tiles into VRAM, sets each of the 64 */
/* drops' sprite words and places it on the ground at the map's focus with its
 */
/* first fall staggered, turns on alpha blending of all layers at 16/16, */
/* schedules the update and, when asked, plays SE_KIRIN (scQueueMusic). */
void fdWeatherStartRain(s32 withCue) {
	WEATHER_STATE *state;
	WEATHER_PARTICLE *particle;
	u8 *tiles;
	u32 i;
	u32 size;
	HEAP_SLOTS *heap;

	size = sizeof(WEATHER_STATE);
	state = syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, size);
	particle = state->particles;
	size = 0;
	btDmaFill32(size, state, sizeof(*state));
	tiles = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLz(FieldEffect_StreakTiles, tiles);
	state->entry = syVramCacheFindFreeEntry();
	/* As many bytes as the motes' six pictures, though streak.png has two. */
	state->tileBase =
		syVramCacheLoad(state->entry, 6 * WEATHER_PICTURE_BYTES, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	i = 0;
	heap = Ram_HeapSlots;
	for(; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
		MAP_STATE *view = heap->mapGrid;
		s32 *focus;
		/* p walks the sprite's attribute words, then holds the x, as size
		 * carries */
		/* the fill's 0 above. */
		u32 p;
		s32 z;

		p = (u32)particle;
		*(s32 *)p = 0;
		p += sizeof(s32);
		focus = view->origin;
		*(s32 *)p = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		p += sizeof(s32);
		*(s32 *)p = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		p = focus[0];
		z = focus[2];
		particle->posX = (s32)p;
		particle->posZ = z;
		particle->posY = fdMapGridGetTerrainHeightStub(0, (s32)p, z);
		/* Timers 1 to 4 and 9 to 12, by the drop's index. */
		particle->tmr = (i & 0xb) + 1;
	}
	syIoWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
	syIoWrite16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
	syIoWrite16(0, &REG_BLDY);
	sySetEvent(fdWeatherUpdateRain, TASK_PASS_FRAME);
	if(withCue != 0)
		scQueueMusic(SE_KIRIN);
}

/* Stops fdWeatherStartRain's particles. Unused: nothing calls */
/* fdWeatherStopRainStub. */
void fdWeatherStopRain(void) { syCloseEvent(fdWeatherUpdateRain); }

/* The sparkles each frame: now and then a burst of four rises from a random */
/* spot within 128 pixels of the map's focus, each a frame of */
/* FieldEffect_SparklePictures every two frames and started 4 frames after the
 */
/* one before; FLAG_WEATHER_HELD holds them where they are. */
void fdEffectUpdateSparkles(void) {
	WEATHER_STATE *state;
	u32 spawned;
	u32 i;
	MAP_STATE *map;
	s32 *camera;
	s32 spawnX;
	s32 spawnZ;
	WEATHER_PARTICLE *sparkle;
	const u16 *frame;
	s32 *leader;
	s32 cameraX;
	s32 cameraZ;
	s32 screenX;
	s32 screenY;
	s32 delay;
	s32 x;
	u32 a;
	u32 b;
	u32 frames;

	state = Ram_HeapSlots->weather;
	spawned = 0;
	map = Ram_HeapSlots->mapGrid;
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
			frame =
				(const u16 *)&FieldEffect_SparklePictures[sparkle->tmr >> 1];
			a = syRandom();
			b = syRandom();
			x = FIXEDtoINT(sparkle->posX - cameraX) +
				(((a & 1) + (b & 1)) >> 1);
			screenX = x - 1;
			screenY = (sparkle->posZ - sparkle->posY - cameraZ) / FIXED_ONE +
					  *(const s16 *)frame;
			frame++;
			if(x >= -15 && x <= DISPLAY_WIDTH && screenY >= -32 &&
			   screenY < DISPLAY_HEIGHT) {
				sparkle->tile = state->tileBase + *frame;
				sparkle->x = screenX;
				sparkle->shape = OBJ_SHAPE_SQUARE;
				sparkle->size = OAM_SIZE_16X16;
#ifdef UBFIX
				frames = FrameCount;
#else
				/* UB: this reads a u32 as unsigned long; UBFIX reads the
				 * declared type. */
				frames = *(unsigned long *)&FrameCount;
#endif
				sparkle->y = screenY;
				sparkle->affineIdx = ((frames >> 1) & 1) << 3;
				syOamAddSprAtDepthClamped(sparkle, OAM_DEPTH_WINDOW);
			}
			sparkle->tmr--;
		}
		if(spawned <= 3 && sparkle->tmr == 0 && state->stopped == 0) {
			if(delay != 0) {
				/* The rest of a burst, on the first sparkle's spot. */
				sparkle->posX = spawnX;
				sparkle->posZ = spawnZ;
				/* BUG: the ground's height is read at the last sparkle drawn's
				 * screen */
				/* place taken as a 16.16 position, near the map's top left
				 * corner, */
				/* rather than under the burst; the same below. */
#ifdef BUGFIX
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
			}
			else if((syRandom() & 255) == 0) {
				/* A burst starts at a random spot. */
				leader = map->origin;
				spawnX = leader[0] + (syRandom() << 8) - toFIXED(128);
				spawnZ = leader[2] + (syRandom() << 8) - toFIXED(128);
				sparkle->posX = spawnX;
				sparkle->posZ = spawnZ;
#ifdef BUGFIX
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

#define syDecompressByteLz ((void (*)(const void *, void *))syDecompressByteLz)

/* Unused: nothing calls fdEffectInitSparklesStub; Aqua Rock and the */
/* Apojii Islands keep their own copy (shared/ground_particles_start.h). Starts
 */
/* the sparkles (fdEffectUpdateSparkles): loads their tiles and lays every */
/* sparkle at the map's top left corner with a staggered timer. */
void fdEffectInitSparkles(void) {
	u32 size = sizeof(WEATHER_STATE);
	WEATHER_STATE *state = syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, size);
	WEATHER_PARTICLE *p = state->particles;
	u8 *buf;
	u32 i;
	s32 clear;
	volatile u32 zero;
	HEAP_SLOTS *heap;

	size = 0;
	btDmaFill32Cell(zero, size, state, sizeof(WEATHER_STATE));
	buf = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLz(FieldEffect_SparkleTiles, buf);
	state->entry = syVramCacheFindFreeEntry();
	state->tileBase =
		syVramCacheLoad(state->entry, 4 * WEATHER_PICTURE_BYTES, buf);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);

	i = 0;
	heap = Ram_HeapSlots;
	clear = 0;
	do {
		MAP_STATE *view = heap->mapGrid;
		s32 *leader;
		/* attr walks the sprite's attribute words, then holds the x. */
		u32 attr;
		s32 z;

		attr = (u32)p;
		*(u32 *)attr = clear;
		attr += sizeof(u32);
		leader = view->origin;
		*(u32 *)attr = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		attr += sizeof(u32);
		*(u32 *)attr = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
		attr = leader[0];
		z = leader[2];
		p->posX = clear;
		p->posZ = clear;
		p->posY = fdMapGridGetTerrainHeightStub(0, attr, z);
		p->tmr = (i & (FIRST_SPAWN_FRAMES - 1)) + 1;
		i++;
		p++;
	} while(i < WEATHER_PARTICLE_COUNT);
	sySetEvent(fdEffectUpdateSparkles, TASK_PASS_FRAME);
}

/* Starts the storm's lightning (fdWeatherUpdateLightning): builds the dark */
/* (PALETTE_TINT_WARM) and lit (PALETTE_TINT_PALE) BG colours and their 12-step
 */
/* fade, and arms the first burst to end in 600 frames, repeating. */
void fdWeatherStartLightning(void) {
	FIELD_BLEND_STATE *state;
	PALETTE_FADE *buffers;
	volatile u32 zero;

	state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = Ram_HeapSlots->palFade;
	btDmaFill32Cell(zero, 0, state, sizeof(*state));
	fdPaletteFadeBuild(PALETTE_TINT_WARM, (u16 *)buffers, (u16 *)state, 1);
	fdPaletteFadeBuild(PALETTE_TINT_PALE, (u16 *)buffers, (u16 *)state->to, 1);
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

/* Restarts the lightning: the next burst ends in 128 frames, and bursts */
/* repeat. */
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
	volatile u32 zero;
	s16 *target;

	state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	buffers = Ram_HeapSlots->palFade;
	btDmaFill32Cell(zero, 0, state, sizeof(*state));
	fdPaletteFadeBuild(from, (u16 *)buffers, (u16 *)state, 1);
	target = state->to;
	fdPaletteFadeBuild(to, (u16 *)buffers, (u16 *)target, 1);
	fdPaletteFadeCalcSteps(target, state->from, state->delta, 12);
	fdPaletteFadeBuild((s32)state, 0, buffers->tgt, 1);
	state->tmr = 120;
	state->enabled = 0;
	sySetEvent(fdWeatherUpdateLightning, TASK_PASS_FRAME);
}

#undef FIRST_SPAWN_FRAMES
