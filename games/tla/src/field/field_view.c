/* Screen shake, the copied BG row drawer and the ground plane scanlines. Part
   of field.c. */

/* A wait for a shake to settle gives up after five seconds, and ends once both
   amplitudes are under 1/256 pixel (16.16). */
#define MAPGRID_SHAKE_SETTLE_TICKS		   300
#define MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE (toFIXED(1) / 256 - 1)
/* One of the world map's two tile maps of one-byte tiles, BG3's and then BG2's,
   as fdWorldMapStartStillImage copies them into DecodeBuffer. */
#define WORLD_MAP_TILE_MAP_BYTES \
	(WORLD_MAP_TILE_MAP_WIDTH * WORLD_MAP_TILE_MAP_WIDTH)

/* The bit of a world map block's top byte, its word's bit 31, that makes it a
   beach. */
#define WORLD_MAP_BLOCK_BEACH (1 << 7)

/* The sets of rows of WorldMapTerrainBehaviorTable, 16 collision values each,
   which differ in the battles their walkable ground and forest give. */
typedef enum TERRAIN_BEHAVIOR_ROWS {
	TERRAIN_ROWS_PLAIN = 0,
	TERRAIN_ROWS_BEACH = 16, /* walkable ground fights on the beach */
	TERRAIN_ROWS_SNOWFIELD = 32,
	TERRAIN_ROWS_DESERT = 48, /* walkable ground fights in the desert */
	TERRAIN_ROWS_BLIZZARD = 64
} TERRAIN_BEHAVIOR_ROWS;

/* The encounter regions (WORLD_MAP_METATILE.region) whose blocks take rows of
   their own. */
enum {
	AIRS_ROCK_DESERT_REGION = 12, /* the desert round Air's Rock */
	/* the sandy shore north of Shaman Village */
	SHAMAN_VILLAGE_SANDS_REGION = 15,
	PROX_BLIZZARD_REGION = 36,	/* the snow round Prox */
	SNOWFIELD_REGION_FIRST = 41 /* and every region after it */
};

/* Returns the world map's battle environment at position from
   WorldMapTerrainBehaviorTable, by its collision value and its block's rows,
   and stores the block's encounter region in encounterRegion. */
Uint8 fdWorldMapGetBattleEnvironment(WORLD_POS *pos, Sint32 *encounterRegion)
{
	Sint32	collisionKind = fdWorldMapGetCollision(pos);
	Sint32	x = pos->x;
	Sint32	y;
	Sint32	rows = TERRAIN_ROWS_PLAIN;
	Uint32	blockX;
	Uint32	idx;
	Uint32 *block;
	Uint32	region;

	blockX = (x / toFIXED(32)) & (WORLD_MAP_VIEW_METATILES - 1);
	y = pos->y;

	idx = blockX + (((y / toFIXED(32)) & (WORLD_MAP_VIEW_METATILES - 1)) << 5);
	block = MetatileTiles + idx;
	if(((Uint8 *)block)[3] & WORLD_MAP_BLOCK_BEACH)
		rows = TERRAIN_ROWS_BEACH;

	region = ((WORLD_MAP_METATILE *)block)->region;
	*encounterRegion = region;
	if(region >= SNOWFIELD_REGION_FIRST)
		rows = TERRAIN_ROWS_SNOWFIELD;
	if(region == PROX_BLIZZARD_REGION)
		rows = TERRAIN_ROWS_BLIZZARD;
	if(region == AIRS_ROCK_DESERT_REGION ||
		region == SHAMAN_VILLAGE_SANDS_REGION)
		rows = TERRAIN_ROWS_DESERT;

	return WorldMapTerrainBehaviorTable[rows + collisionKind];
}

/* Changes the shake without restarting an axis whose argument is negative.
   Amplitudes are 16.16 pixels and decay is a 16.16 multiplier per tick. */
void fdMapGridSetScreenShake(FIXED shakeX, FIXED shakeY, FIXED shakeDecay)
{
	MAP_STATE *grid;

	/* A negative argument leaves that value alone, so a script can change the
	   decay without restarting the shake, or start one axis only. */
	grid = Ram_HeapSlots->mapGrid;
	if(shakeX >= 0) {
		grid->shakeX = shakeX;
	}
	if(shakeY >= 0) {
		grid->shakeY = shakeY;
	}
	if(shakeDecay >= 0) {
		grid->shakeDecay = shakeDecay;
	}
}

/* Waits until both shake amplitudes are below 1/256 pixel, with a 300-tick
   limit for a shake whose decay never settles. */
void fdMapGridWaitForShakeToSettle(void)
{
	MAP_STATE *grid;
	Sint32	   waitedTicks;

	grid = Ram_HeapSlots->mapGrid;
	/* 300 ticks is five seconds; 255 in 16.16 is just under 1/256 pixel. A
	   decay of 1.0 or more never settles, so the limit ends the wait. */
	for(waitedTicks = 0; waitedTicks < MAPGRID_SHAKE_SETTLE_TICKS &&
		(grid->shakeX > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE ||
			grid->shakeY > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE);
		waitedTicks++) {
		sySynch(1);
	}
	/* A zero decay makes the next view update's shake its last. */
	grid->shakeDecay = 0;
}

/* The ARM function copied to RAM reads the BG2 affine row from each 32-byte
   pair and writes 240 by 160 pixels as 256-colour tiles; it returns the address
   past the last pair, where its row cursor stops. */
typedef const WORLD_MAP_AFFINE_PAIR *(*TILE_MAP_ROW_DRAWER)(
	const WORLD_MAP_AFFINE_PAIR *lines, Uint8 *pixels, const Uint8 *tiles,
	const Uint8 *map);

/* Draws the current affine page into the world map's still-image pixels.
   fdWorldMapStartStillImage supplies 160 pairs and a 240 by 160 byte output. */
void fdTileMapDrawRowsFromRam(const WORLD_MAP_AFFINE_PAIR *lines, Uint8 *pixels)
{
	TILE_MAP_ROW_DRAWER routine;
	Uint8			   *maps = DecodeBuffer;

	/* The second tile set and tile map are BG2's. */
	RAM_CODE_LOAD(routine, fdRenderDrawMapCode, fdRenderDrawMapCodeSize);
	routine(
		lines, pixels, WORLD_MAP_TILE_SET(2), maps + WORLD_MAP_TILE_MAP_BYTES);
	syHeapRelease(routine);
}

/* Rebuilds the ground plane's scanline terms when the world map pitch
   changes. */
void fdWorldMapBuildScanlineTable(
	FIXED pitchSlope, Sint32 *pos, WORLD_MAP_SCANLINE *scanline)
{
	Sint32 groundPoint[3];
	Sint32 cameraPoint[3];
	FIXED  horizon;
	FIXED  focus;
	Sint32 line;
	Sint32 slope;
	FIXED  scale;
	Sint32 horizontal;
	FIXED  squaredSum;
	Sint32 groundHeight = 0;

	/* The followed point on the ground (height 0), in camera space; horizon is
	   its height less its depth times the pitch slope. */
	groundPoint[0] = pos[0];
	groundPoint[1] = groundHeight;
	groundPoint[2] = pos[2];
	syCalcPoint(groundPoint, cameraPoint);
	horizon = cameraPoint[1] - syMulFX(cameraPoint[2], pitchSlope);
	focus = -ProjectionWork.focal;
	for(line = 0; line < DISPLAY_HEIGHT; line++) {
		FIXED difference;

		/* The line's ray slope: its offset from the projection's centre line
		   over the focal length; later the vertical term. */
		slope = syDivFX(focus, (ProjectionWork.centerY - line) * toFIXED(1));
		difference = slope - pitchSlope;
		if(difference == 0)
			difference = 1;
		scale = syDivFX(difference, horizon);
		/* A negative scale is a line below the horizon, whose ray meets the
		   ground; the lines above it are sky and get zeros. */
		if(scale < 0) {
			/* The width scale is half the line's scale over the focal length,
			   and the depth is stored halved. */
			scanline->scale =
				syDivFX(ProjectionWork.focal, syMulFX(-scale, toFIXED(1) / 2));
			slope = syMulFX(scale, slope);
			horizontal = (cameraPoint[2] - scale) >> 4;
			slope -= cameraPoint[1];
			squaredSum = syMulFX(horizontal, horizontal);
			slope >>= 4;
			squaredSum += syMulFX(slope, slope);
			{
				/* The root of a 16.16 square comes back in 8.8, and the terms
				   were taken in sixteenths. */
				Sint32 groundDistance = sySquartIWRAM(squaredSum) << (8 + 4);

				if(slope < 0)
					groundDistance = -groundDistance;
				scanline->depth = syMulFX(groundDistance, toFIXED(1) / 2);
			}
		} else {
			scanline->scale = 0;
			scanline->depth = 0;
		}
		scanline->ofsX = 0;
		scanline->ofsY = 0;
		scanline++;
	}
}
