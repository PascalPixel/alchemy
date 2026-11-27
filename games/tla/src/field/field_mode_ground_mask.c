/* The ground mask: 16-pixel sprites kept beside an actor where the ground
   before it rises, in a fade form nothing starts and a blank form Aqua Rock
   starts, which draws no visible pixel; and the effect sprites that borrow
   their tiles. Part of field_mode.c. */

/* A screen row's map metatile, in pixels, in an OAM entry's 8-bit y, as
   MAPGRID_METATILE_PIXEL_MASK is for a whole pixel coordinate. */
#define METATILE_GRID_ROW_MASK 0xf0
/* MAP_METATILE.attributeB's lowest bit, on the map's third layer: a metatile
   the blank overlay marks. The marks' tiles are colour 0, transparent (GBATEK
   "Color Palette RAM", Transparent Colors), so the player sees nothing of
   them. */
#define METATILE_BLANK_OVERLAY 0x01

/* Each frame of the fade overlay: on each of the two metatiles beside the actor
   (left and right of it), where the ground one or two metatiles south rises
   above the actor's floor, draws a semi-transparent 16-pixel-wide sprite of
   colour 1 at the height of that ground. */
void fdFadeOverlayDrawMarks(void)
{
	FADE_OVERLAY_STATE *state = Ram_HeapSlots->fadeOverlay;
	Sint32				cameraX =
		(Sint16)FIXEDtoINT(syMapStateView(Ram_HeapSlots->mapGrid)[0]);
	Sint32 cameraZ =
		(Sint16)FIXEDtoINT(syMapStateView(Ram_HeapSlots->mapGrid)[1]);
	ANIMATION_SPR_PART *mark = state->marks;
	FADE_ACTOR		   *actor = state->actor;
	FIXED				x;
	FIXED				z;
	Sint32				terrainFloor;
	Uint8				terrainLayer;
	Uint32				tile;
	Sint32				height;
	Sint32				secondHeight;
	Sint32				screenRow;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint16 unused[2];

	if(actor == 0)
		return;
	z = actor->z;
	x = actor->x;
	terrainFloor = actor->floor;
	terrainLayer = actor->terrainId;
	/* The first tile of the solid block fdEffectStartFadeOverlay loaded. */
	tile = VramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].ofs / TILE_SIZE_4BPP;
	/* The left metatile, 8 pixels left of the actor. */
	x -= toFIXED(8);
	height = FIXEDtoINT(
		fdMapGridGetTerrainHeightStub(terrainLayer, x, z + toFIXED(16)));
	secondHeight = FIXEDtoINT(fdMapGridGetTerrainHeightStub(
					   terrainLayer, x, z + toFIXED(32))) -
		16;
	if(secondHeight > height)
		height = secondHeight;
	if(height > 0 && height > terrainFloor) {
		/* The sprite mode and priority set next replace what the two words put
		   there, so the left and right marks end up alike although their third
		   words differ. */
		((Uint32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
		((Uint32 *)mark)[2] = OBJ_PRIORITY(1);
		mark->pri = 0;
		mark->tile = tile;
		mark->objMode = OBJ_MODE_SEMI_TRANSPARENT;
		/* Snapped to the metatile's grid, then moved up by the ground's
		   height. */
		mark->x =
			((Uint32)FIXEDtoINT(x) & MAPGRID_METATILE_PIXEL_MASK) - cameraX;
		screenRow = FIXEDtoINT(z) & METATILE_GRID_ROW_MASK;
		mark->y = screenRow - cameraZ - height + 16;
		syOAMAddSprAtDepthClamped(mark, 0);
	}
	/* The right metatile, 8 pixels right of the actor, read the same way. */
	x += toFIXED(16);
	height = FIXEDtoINT(
		fdMapGridGetTerrainHeightStub(terrainLayer, x, z + toFIXED(16)));
	mark = &state->marks[1];
	secondHeight = FIXEDtoINT(fdMapGridGetTerrainHeightStub(
					   terrainLayer, x, z + toFIXED(32))) -
		16;
	if(secondHeight > height)
		height = secondHeight;
	if(height > 0 && height > terrainFloor) {
		((Uint32 *)mark)[1] = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
		((Uint32 *)mark)[2] = 0;
		mark->pri = 0;
		mark->tile = tile;
		mark->objMode = OBJ_MODE_SEMI_TRANSPARENT;
		mark->x =
			((Uint32)FIXEDtoINT(x) & MAPGRID_METATILE_PIXEL_MASK) - cameraX;
		screenRow = FIXEDtoINT(z) & METATILE_GRID_ROW_MASK;
		mark->y = screenRow - cameraZ - height + 16;
		syOAMAddSprAtDepthClamped(mark, 0);
	}
}

/* Starts the fade overlay on an actor (the party leader when NULL): loads its
   solid tiles, sets the blending and draws it each frame. Unused: nothing calls
   it or its stub call, fdEffectStartFadeOverlayStub. */
void fdEffectStartFadeOverlay(void *actor)
{
	FADE_OVERLAY_STATE *state;
	Uint8			   *tiles;
	volatile Uint32		fill;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_FADE_OVERLAY, sizeof(*state));
	tiles = syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH, 1024);
	DMA_FILL32_CELL(fill, PIXELS_4BPP(1), tiles, 512);
	syVRAMCacheLoad(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, tiles);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	sySetEvent(fdFadeOverlayDrawMarks, EVENT_PASS_FRAME);
	syIOWrite16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
			BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
		&REG_BLDCNT);
	syIOWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
	/* 31 is above the register's useful maximum, BLEND_FADE_MAX, and acts as
	   it. */
	syIOWrite16(31, &REG_BLDY);
	DMA_FILL32_CELL(fill, 0, state, sizeof(*state));
	if(actor == NULL)
		actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	state->actor = actor;
}

/* Stops drawing the fade overlay, keeping its state. */
void fdFadeOverlaySuspend(void)
{
	sySuspendEvent(fdFadeOverlayDrawMarks);
}

/* Draws the fade overlay again after fdFadeOverlaySuspend. */
void fdFadeOverlayResume(void)
{
	syResumeEvent(fdFadeOverlayDrawMarks);
}

/* The blank overlay's event, which fdEffectStartBlankOverlay starts: each frame
   it draws a 16-pixel mark beside the actor where the map's third layer flags a
   metatile two rows up (left or right of the actor), and another for the row
   one up. */
void fdBlankOverlayDrawMarks(void)
{
	ANIMATION_SPR_PART *mark = Ram_HeapSlots->fadeOverlay;
	MAP_STATE		   *map = Ram_HeapSlots->mapGrid;
	Sint16				cameraX = FIXEDtoINT(syMapStateView(map)[0]);
	Sint16				cameraZ = FIXEDtoINT(syMapStateView(map)[1]);
	FADE_ACTOR		   *actor = ((FADE_OVERLAY_STATE *)mark)->actor;
	Sint32				x;
	Sint32				row;
	Uint32				tile;
	MAP_METATILE	   *metatiles;
	MAP_METATILE	   *right;
	MAP_METATILE	   *left;
	ANIMATION_SPR_PART *m;

	if(actor == 0)
		return;
	x = ((Sint16 *)&actor->x)[1];
	row = FIXEDtoINT(actor->z - actor->y);
	tile = VramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].ofs / TILE_SIZE_4BPP;
	metatiles = map->layers[2].metatiles;
	right = metatiles + (x + 8) / MAPGRID_METATILE_PIXELS +
		(row / MAPGRID_METATILE_PIXELS - 2) * MAPGRID_SIZE;
	left = metatiles + (x - 8) / MAPGRID_METATILE_PIXELS +
		(row / MAPGRID_METATILE_PIXELS - 2) * MAPGRID_SIZE;
	if((right->attributeB & METATILE_BLANK_OVERLAY) ||
		(left->attributeB & METATILE_BLANK_OVERLAY)) {
		m = mark;
		((Uint32 *)m)[1] = OBJ_SIZE_16X16;
		((Uint32 *)m)[2] = OBJ_PRIORITY(1);
		m->tile = tile;
		m->x = (Uint16)(x - cameraX) - 8;
		m->y = (Uint16)(((Uint16)row & METATILE_GRID_ROW_MASK) - cameraZ) - 32;
		mark = m + 1;
		syOAMAddSprAtDepthClamped(m, 1);
	}
	right = metatiles + (x + 8) / MAPGRID_METATILE_PIXELS +
		(row / MAPGRID_METATILE_PIXELS - 1) * MAPGRID_SIZE;
	left = metatiles + (x - 8) / MAPGRID_METATILE_PIXELS +
		(row / MAPGRID_METATILE_PIXELS - 1) * MAPGRID_SIZE;
	if((right->attributeB & METATILE_BLANK_OVERLAY) ||
		(left->attributeB & METATILE_BLANK_OVERLAY)) {
		m = mark;
		((Uint32 *)m)[1] = OBJ_SIZE_16X16;
		((Uint32 *)m)[2] = OBJ_PRIORITY(1);
		m->tile = tile;
		m->x = (Uint16)(x - cameraX) - 8;
		m->y = (Uint16)(((Uint16)row & METATILE_GRID_ROW_MASK) - cameraZ) - 16;
		syOAMAddSprAtDepthClamped(m, 1);
	}
}

/* Like fdEffectStartFadeOverlay with blank tiles and no blending:
   loads 512 bytes of blank tiles into VRAM_CACHE_ENTRY_FADE_OVERLAY and starts
   fdBlankOverlayDrawMarks on an actor (the party leader when 0). Aqua Rock's
   rooms start it; its marks are all colour 0, so nothing shows. */
void fdEffectStartBlankOverlay(Sint32 actor)
{
	FADE_OVERLAY_STATE *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FADE_OVERLAY, sizeof(*state));
	void *buffer = syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH, 1024);

	DMA_FILL32(0, buffer, 512);
	syVRAMCacheLoad(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, buffer);
	sySetEvent(fdBlankOverlayDrawMarks, EVENT_PASS_FRAME);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	if(actor == 0)
		actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	state->actor = (FADE_ACTOR *)actor;
}

/* Suspends the fdBlankOverlayDrawMarks overlay event. */
void fdBlankOverlaySuspend(void)
{
	sySuspendEvent(fdBlankOverlayDrawMarks);
}

/* Resumes the fdBlankOverlayDrawMarks overlay event. */
void fdBlankOverlayResume(void)
{
	syResumeEvent(fdBlankOverlayDrawMarks);
}

/* Fills the overlay's 512 bytes of sprite tiles (VRAM_CACHE_ENTRY_FADE_OVERLAY)
   with PIXELS_8BPP(1) when solid is set, which in these 16-colour tiles is
   colour 1 in every other column (no caller sets it), else clears them. */
void fdFadeOverlayFillTiles(Sint32 solid)
{
	void *buffer = syHeapSlotAlloc(HEAP_SLOT_TILE_SCRATCH, 1024);

	if(solid == 0) {
		DMA_FILL32(0, buffer, 512);
	} else {
		DMA_FILL32(PIXELS_8BPP(1), buffer, 512);
	}
	syVRAMCacheLoad(VRAM_CACHE_ENTRY_FADE_OVERLAY, 512, buffer);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
}

/* Sets up sprite through fdEffectSprInit with the overlay's tiles
   (VRAM_CACHE_ENTRY_FADE_OVERLAY), at priority 1 in palette 0. */
void fdEffectSprInitWithFadeOverlay(
	EFFECT_SPR *sprite, Sint32 pivotX, Sint32 pivotY, Uint32 attributes)
{
	fdEffectSprInit(sprite, pivotX, pivotY, attributes,
		VramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].ofs / TILE_SIZE_4BPP);
	sprite->depth = 0;
	sprite->oam.pri = 1;
	sprite->oam.fullCol = 0;
	sprite->oam.pal = 0;
}

/* Projects the world position (x, y, z) to the screen, stores it as the effect
   sprite's position and draws the sprite. */
void fdEffectSprDrawAtWorldXYZ(EFFECT_SPR *sprite, Sint32 x, Sint32 y, Sint32 z)
{
	Sint32 position[3];

	SET_VECTOR(position, x, y, z);
	fdCameraWorldToScreen(position);
	sprite->x = position[0];
	sprite->y = position[2];
	fdEffectSprDraw(sprite);
}

/* As fdEffectSprInitWithFadeOverlay, semi-transparent and at priority 0. */
void fdEffectSprInitWithFadeOverlayBlended(
	EFFECT_SPR *sprite, Sint32 pivotX, Sint32 pivotY, Uint32 attributes)
{
	fdEffectSprInit(sprite, pivotX, pivotY, attributes,
		VramCacheEntries[VRAM_CACHE_ENTRY_FADE_OVERLAY].ofs / TILE_SIZE_4BPP);
	sprite->depth = 0;
	sprite->oam.pri = 0;
	sprite->oam.fullCol = 0;
	sprite->oam.pal = 0;
	sprite->oam.objMode = OBJ_MODE_SEMI_TRANSPARENT;
}

#undef METATILE_GRID_ROW_MASK
#undef METATILE_BLANK_OVERLAY
