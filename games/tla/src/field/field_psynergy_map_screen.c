/* The map screen: its markers and cursor (The Broken Seal's
   script_commands_map_screen.c), and where the leader stands on it. Part of
   field_psynergy.c. */

/* The generated marker pictures: four 16-colour 8 by 8 tiles, 32 bytes each. */
#define WORLD_MAP_MARKER_TILE_BYTES (4 * TILE_SIZE_4BPP)
/* The word of a marker tile's pixel row, eight 4-bit pixels a word. */
#define MARKER_ROW_WORD(tile, row)	((tile) * (TILE_SIZE_4BPP / 4) + (row))
/* The first five of a marker tile row's eight 4-bit pixels, the leftmost in the
   low bits; the rest are 0. */
#define MAP_SCREEN_MARKER_ROW(p0, p1, p2, p3, p4) \
	((p0) | (p1) << 4 | (p2) << 8 | (p3) << 12 | (p4) << 16)

/* The bytes reserved for the map screen's marker tiles. */
/* BUG: the original reserves 32 bytes but writes and uploads 128, as The Broken
   Seal's does. The bump allocator takes bytes, not words; reserve the whole
   tile buffer with BUGFIX. */
#define WORLD_MAP_MARKER_ALLOCATION_SIZE \
	WHEN(FIX_MAP_MARKER_TILE_ALLOCATION, WORLD_MAP_MARKER_TILE_BYTES, 32)

/* Sets up the map screen: the marker tiles and their OAM entries, the cursor
   where the leader is on the map (or mid-screen when the area is not on it),
   and the name window. */
void fdMapScreenInit(void)
{
	Uint32			*markerTiles;
	WORLD_MAP_STATE *state = Ram_WorldMapState;
	Uint32			*oam;
	FIXED			 screenX;
	FIXED			 screenY;
	Sint32			 tileAttributes;
	Uint32			 i;
	Uint32			 tile;
	Uint32			 cursorAttributes;
	FIXED			 right;
	FIXED			 bottom;
	Sint32			 left;

	markerTiles = syHeapAllocEWRAM(WORLD_MAP_MARKER_ALLOCATION_SIZE);
	oam = (Uint32 *)state->markers;
	/* DEAD: the leader looked up and never used; the ROM keeps the calls. */
	fdActorGetById(fdPartyGetLeaderActor());
	screenX = 0;
	screenY = 0;
	state->vramBlock = syVRAMCacheFindFreeEntry();
	state->searchX = 0;
	state->searchZ = 0;
	DMA_FILL32(0, markerTiles, WORLD_MAP_MARKER_TILE_BYTES);
	/* Each marker tile holds one dot in its top-left corner, with a shadow in
	   colour 1 to its right and below (# the dot, + the shadow):

		 tiles 0 to 2      tile 3
		   ##.               .##..
		   ##+               ####.
		   .++               ####+
							 .##++
							 ..++.

	   Tile 0, in colour 15, marks where the party is; tiles 1 and 2, in colours
	   4 and 7, a place by the marker its row asks for, and tile 2 the ship;
	   tile 3, in colour 15, is a larger dot. */
	markerTiles[MARKER_ROW_WORD(0, 0)] = MAP_SCREEN_MARKER_ROW(15, 15, 0, 0, 0);
	markerTiles[MARKER_ROW_WORD(0, 1)] = MAP_SCREEN_MARKER_ROW(15, 15, 1, 0, 0);
	markerTiles[MARKER_ROW_WORD(0, 2)] = MAP_SCREEN_MARKER_ROW(0, 1, 1, 0, 0);
	markerTiles[MARKER_ROW_WORD(1, 0)] = MAP_SCREEN_MARKER_ROW(4, 4, 0, 0, 0);
	markerTiles[MARKER_ROW_WORD(1, 1)] = MAP_SCREEN_MARKER_ROW(4, 4, 1, 0, 0);
	markerTiles[MARKER_ROW_WORD(1, 2)] = MAP_SCREEN_MARKER_ROW(0, 1, 1, 0, 0);
	markerTiles[MARKER_ROW_WORD(2, 0)] = MAP_SCREEN_MARKER_ROW(7, 7, 0, 0, 0);
	markerTiles[MARKER_ROW_WORD(2, 1)] = MAP_SCREEN_MARKER_ROW(7, 7, 1, 0, 0);
	markerTiles[MARKER_ROW_WORD(2, 2)] = MAP_SCREEN_MARKER_ROW(0, 1, 1, 0, 0);
	markerTiles[MARKER_ROW_WORD(3, 0)] = MAP_SCREEN_MARKER_ROW(0, 15, 15, 0, 0);
	markerTiles[MARKER_ROW_WORD(3, 1)] =
		MAP_SCREEN_MARKER_ROW(15, 15, 15, 15, 0);
	markerTiles[MARKER_ROW_WORD(3, 2)] =
		MAP_SCREEN_MARKER_ROW(15, 15, 15, 15, 1);
	markerTiles[MARKER_ROW_WORD(3, 3)] = MAP_SCREEN_MARKER_ROW(0, 15, 15, 1, 1);
	markerTiles[MARKER_ROW_WORD(3, 4)] = MAP_SCREEN_MARKER_ROW(0, 0, 1, 1, 0);
	tileAttributes = syVRAMCacheLoad(state->vramBlock,
						 WORLD_MAP_MARKER_TILE_BYTES, markerTiles) |
		OBJ_PRIORITY(1);
	for(i = 0; i < MAP_SCREEN_PLACE_COUNT; i++) {
		*oam++ = 0;
		*oam++ = 0;
		*oam++ = tileAttributes;
	}
	syHeapRelease(markerTiles);
	if(syGameFlagGet(FLAG_WORLD_MAP_LEFT)) {
		state->x = toFIXED(264);
		state->z = toFIXED(16);
	} else {
		fdMapScreenGetLeaderPoint(&screenX, &screenY);
		state->x = toFIXED(screenX);
		state->z = toFIXED(screenY);
	}
	screenX = state->x - toFIXED(DISPLAY_WIDTH / 2);
	screenY = state->z - toFIXED(DISPLAY_HEIGHT / 2);
	left = 0;
	right = toFIXED(272);
	bottom = toFIXED(224);
	if(screenX < left)
		screenX = left;
	if(screenX > right)
		screenX = right;
	if(screenY < left)
		screenY = left;
	if(screenY > bottom)
		screenY = bottom;
	Ram_WorldMapCursorStart[0] = state->x;
	Ram_WorldMapCursorStart[1] = state->z;
	state->winSlot = mnPointingHandLoadIntoFreeSlotStub();
	tile = VramCacheEntries[state->winSlot].ofs;
	state->win = mnWindowCreateStub(0, 0, 0, 0, WINDOW_FLAG_INSTANT);
	state->shown = -1;
	state->speed = toFIXED(1);
	cursorAttributes = OBJ_SIZE_16X16;
	*oam++ = 0;
	*oam++ = cursorAttributes;
	cursorAttributes = OBJ_PRIORITY(1);
	cursorAttributes |= tile >> 5;
	*oam = cursorAttributes;
	*Ram_WorldMapCursor = &state->x;
	Ram_WorldMapScroll[0] = screenX;
	Ram_WorldMapScroll[1] = screenY;
}

/* Closes the map screen: returns its VRAM blocks and removes the name
   window. */
void fdMapScreenRelease(void)
{
	syVRAMCacheFreeEntry(WorldMapStateWork.vramBlock);
	syVRAMCacheFreeEntry(WorldMapStateWork.winSlot);
	mnWindowCloseStub((Uint32)WorldMapStateWork.win, 2);
}

/* Unused: nothing calls it. Puts the leader at the world map position of image
   point (x, z), the inverse of fdMapScreenGetLeaderPoint's scale, and the
   camera on it. */
void fdMapScreenPlaceLeaderAt(Sint32 x, Sint32 z)
{
	Sint32	owner = fdPartyGetLeaderActor();
	Sint32 *camera = *(Sint32 **)Ram_HeapSlots->mapGrid;
	ACTOR  *actor = fdActorLookup(owner);

	if(actor != NULL) {
		x = FIXEDtoINT(x);
		x = (x * 16384 + 269 * 16384) / 853;
		z = FIXEDtoINT(z);
		actor->x.fixed = toFIXED(x);
		actor->z.fixed = toFIXED((z * 16384 + 112 * 16384) / 640);
		camera[0] = actor->x.fixed;
		camera[2] = actor->z.fixed;
	}
}

/* The leader's point on the map screen: its world map position scaled by 853
   and 640 in 16384, a far corner of the map folded in
   (FLAG_LEADER_OFF_MAP_SCREEN set while the leader is there); off the world
   map, the image's fixed point (264, 16). */
void fdMapScreenGetLeaderPoint(Sint32 *screenX, Sint32 *screenY)
{
	Sint32		 leader = fdPartyGetLeaderActor();
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR		*actor;
	Sint32		 x;
	Sint32		 z;

	syGameFlagClear(FLAG_LEADER_OFF_MAP_SCREEN);
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
		actor = fdActorLookup(leader);
		if(actor == 0)
			return;
		x = actor->x.part[0].integer;
		z = actor->z.part[0].integer;
		if(z < 1832) {
			if(x < 12000) {
				syGameFlagSet(FLAG_LEADER_OFF_MAP_SCREEN);
				/* In every edition but the Japanese this case is offset from
				   the leader's position; the Japanese release puts it at one
				   fixed point. */
#if MAP_SCREEN_OFF_MAP_POINT_FOLLOWS_LEADER
				x += 65;
				z += 1966;
#else
				x = 8872;
				z = 3320;
#endif
			} else {
				x -= 627;
				z += 6084;
			}
		}
		*screenX = MAP_SCREEN_X(x);
		*screenY = MAP_SCREEN_Y(z);
	} else {
		*screenX = 264;
		*screenY = 16;
	}
}

/* Returns a World Map place's marker: the high nibble of its value when
   MapScreen_FirstListOnly is set, else the low nibble (0 and 15 show none). */
Uint32 fdWorldMapPlaceGetMarker(const WORLD_MAP_PLACE *place)
{
	if(MapScreen_FirstListOnly != 0)
		return place->value >> 4;
	return place->value & 0xf;
}
