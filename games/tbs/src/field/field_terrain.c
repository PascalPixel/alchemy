/* Ground heights, and the collision of metatiles and the world map. Part of
   field.c. */

#include "field_internal.h"

/* A sample is in eight-pixel units. */
#define CURVE_HEIGHT_FIXED(sample) ((sample) * (1 << CURVE_VALUE_SHIFT))

/* The ground height functions (MapGridTerrainHeightFunctions): each takes a
   metatile's height samples after its kind byte and a pixel (x, y) within the
   16x16 metatile, and returns the ground height there in 16.16 pixels. */
FIXED fdTerrainGetFlatHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	return CURVE_HEIGHT_FIXED(samples[0]);
}

/* The slopes along one axis: from sample 0 where `along`, the pixel's
   coordinate on that axis, is 0, towards sample 1 where it is 16, rounded
   towards zero as dividing by 16 would. */
#define TERRAIN_DEFINE_SLOPE(name, along) \
	Sint32 name(const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY) \
	{ \
		const Sint8 *samples = (const Sint8 *)heightBytes; \
		Sint32		 startHeight; \
		Sint32		 heightDelta; \
\
		startHeight = CURVE_HEIGHT_FIXED(samples[0]); \
		heightDelta = \
			(CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * (along); \
		return startHeight + heightDelta / CURVE_FULL_STEPS; \
	}

/* A slope along x. */
TERRAIN_DEFINE_SLOPE(fdTerrainGetSlopeXHeight, pixelX)
/* A slope along y. */
TERRAIN_DEFINE_SLOPE(fdTerrainGetSlopeYHeight, pixelY)

/* A step across the diagonal x + y = 15: sample 0 before it, sample 1 after it
   and the higher of the two on it. */
FIXED fdTerrainGetStepSumDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	Sint32		 firstHeight;
	Sint32		 secondHeight;
	Sint32		 higherHeight;
	Uint32		 summedPixel;

	firstHeight = CURVE_HEIGHT_FIXED(samples[0]);
	secondHeight = CURVE_HEIGHT_FIXED(samples[1]);
	higherHeight = firstHeight;
	if(secondHeight > firstHeight) {
		higherHeight = secondHeight;
	}
	/* x + y runs from 0 to 30; 15 is the diagonal. */
	summedPixel = x + y;
	if(summedPixel == CURVE_FULL_STEPS - 1) {
		return higherHeight;
	}
	if(summedPixel < CURVE_FULL_STEPS - 1) {
		secondHeight = firstHeight;
	}
	return secondHeight;
}

/* A step across the diagonal x = y: sample 0 where y < x, sample 1 where y > x
   and the higher of the two on it. */
FIXED fdTerrainGetStepDifferenceDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	Sint32		 firstHeight;
	Sint32		 secondHeight;
	Sint32		 higherHeight;
	Sint32		 pixelDifference;
	Uint32		 biasedDifference = x;

	firstHeight = CURVE_HEIGHT_FIXED(samples[0]);
	secondHeight = CURVE_HEIGHT_FIXED(samples[1]);
	higherHeight = firstHeight;
	if(secondHeight > firstHeight) {
		higherHeight = secondHeight;
	}
	/* y - x biased by 15 runs from 0 to 30, so the diagonal is 15 and one
	   unsigned compare tells the sides apart, as in
	   TERRAIN_STEP_SUM_DIAGONAL. */
	pixelDifference = y - biasedDifference;
	biasedDifference = pixelDifference + CURVE_FULL_STEPS - 1;
	if(biasedDifference == CURVE_FULL_STEPS - 1) {
		return higherHeight;
	}
	if(biasedDifference < CURVE_FULL_STEPS - 1) {
		secondHeight = firstHeight;
	}
	return secondHeight;
}

/* A slope that bends on the diagonal x + y = 15: from sample 0 at the top left
   corner to sample 1 on the diagonal to sample 2 at the bottom right. */
FIXED fdTerrainGetBendSumDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	Sint32		 firstHeight;
	Sint32		 middleHeight;
	Sint32		 lastHeight;
	Sint32		 height;

	firstHeight = CURVE_HEIGHT_FIXED(*samples++);
	middleHeight = CURVE_HEIGHT_FIXED(*samples);
	lastHeight = CURVE_HEIGHT_FIXED(samples[1]);
	/* x + y from 0 to 30: 15 is the diagonal, below it the first half and above
	   it the second. */
	x += y;
	if(x == CURVE_FULL_STEPS - 1) {
		height = middleHeight;
	} else if((Uint32)x < CURVE_FULL_STEPS - 1) {
		height = firstHeight +
			(middleHeight - firstHeight) * x / (CURVE_FULL_STEPS - 1);
	} else {
		x -= (CURVE_FULL_STEPS - 1);
		height = middleHeight +
			(lastHeight - middleHeight) * x / (CURVE_FULL_STEPS - 1);
	}
	return height;
}

/* A slope that bends on the diagonal x = y: from sample 0 at the top right
   corner to sample 1 on the diagonal to sample 2 at the bottom left. */
FIXED fdTerrainGetBendDifferenceDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	Sint32		 firstHeight;
	Sint32		 middleHeight;
	Sint32		 lastHeight;
	Sint32		 pixelDifference;
	Sint32		 heightDelta;

	firstHeight = CURVE_HEIGHT_FIXED(*samples++);
	middleHeight = CURVE_HEIGHT_FIXED(samples[0]);
	lastHeight = samples[1];
	/* As TERRAIN_BEND_SUM_DIAGONAL, on y - x biased by 15. */
	pixelDifference = y - x;
	x = pixelDifference + CURVE_FULL_STEPS - 1;
	lastHeight = CURVE_HEIGHT_FIXED(lastHeight);

	if(x == CURVE_FULL_STEPS - 1)
		return middleHeight;

	if((Uint32)x < CURVE_FULL_STEPS - 1) {
		heightDelta = middleHeight - firstHeight;
		pixelDifference = heightDelta * x / CURVE_FULL_STEPS;
		return firstHeight + pixelDifference;
	}
	/* The second half measures from the diagonal by y - x itself, 1 to 15. */
	pixelDifference *= lastHeight - middleHeight;
	return middleHeight + pixelDifference / CURVE_FULL_STEPS;
}

/* A raised or sunken rounded square: sample 1 where TerrainRoundedSquareMask
   is 1, sample 0 in the corners. */
FIXED fdTerrainGetRoundedSquareHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	Sint32		 startHeight;

	startHeight = CURVE_HEIGHT_FIXED(samples[0]);
	/* The weight is 0 or 1, so the result is one sample or the other, never
	   between. */
	return startHeight +
		((CURVE_HEIGHT_FIXED(samples[1]) - startHeight) *
			TerrainRoundedSquareMask[x + (y * CURVE_FULL_STEPS)]);
}

/* The steps along one axis: sample 0 where `along`, the pixel's coordinate on
   that axis, is below 8, sample 1 from 8. */
#define TERRAIN_DEFINE_STEP(name, along) \
	Sint32 name(const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY) \
	{ \
		const Sint8 *samples = (const Sint8 *)heightBytes; \
		Sint32		 heightSample; \
		if((Uint32)(along) < CURVE_HALF_STEPS) \
			heightSample = samples[0]; \
		else \
			heightSample = samples[1]; \
		return CURVE_HEIGHT_FIXED(heightSample); \
	}

/* A step along x. */
TERRAIN_DEFINE_STEP(fdTerrainGetStepXHeight, pixelX)
/* A step along y. */
TERRAIN_DEFINE_STEP(fdTerrainGetStepYHeight, pixelY)

/* A slope along x that bends at the middle: sample 0 at x = 0, sample 1 at x =
   8, towards sample 2 at x = 16. */
FIXED fdTerrainGetBendXHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	Sint32		 endpointHeight;
	Sint32		 middleHeight;

	endpointHeight = CURVE_HEIGHT_FIXED(*samples++);
	middleHeight = CURVE_HEIGHT_FIXED(*samples++);
	/* Each half is 8 pixels; the divisions round towards zero. */
	if((Uint32)pixelX < CURVE_HALF_STEPS)
		return endpointHeight +
			(middleHeight - endpointHeight) * pixelX / CURVE_HALF_STEPS;
	endpointHeight = CURVE_HEIGHT_FIXED(*samples);
	return middleHeight +
		(endpointHeight - middleHeight) * (pixelX - CURVE_HALF_STEPS) /
		CURVE_HALF_STEPS;
}

/* A slope along y that bends at the middle, as TERRAIN_BEND_X does along x. */
FIXED fdTerrainGetBendYHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY)
{
	const Uint8 *samples = heightBytes;
	Uint32		 metatileY = pixelY;
	Sint32		 endpointHeight;
	Sint32		 middleHeight;
	Sint32		 ofsPastMiddle;
	Sint32		 heightDelta;
	Sint32		 tailDelta;

	endpointHeight = CURVE_HEIGHT_FIXED(*samples++);
	middleHeight = CURVE_HEIGHT_FIXED(*samples++);
	/* As TERRAIN_BEND_X: halves of 8 pixels, rounded towards zero. */
	if(metatileY < CURVE_HALF_STEPS) {
		heightDelta = (middleHeight - endpointHeight) * metatileY;
		return endpointHeight + heightDelta / CURVE_HALF_STEPS;
	}

	endpointHeight = CURVE_HEIGHT_FIXED(*samples);
	ofsPastMiddle = metatileY;
	endpointHeight -= middleHeight;
	ofsPastMiddle -= CURVE_HALF_STEPS;
	tailDelta = endpointHeight * ofsPastMiddle;
	return middleHeight + tailDelta / CURVE_HALF_STEPS;
}

/* The sample TerrainSampleIndexTable picks for (x, y). */
FIXED fdTerrainGetSampleTableHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	/* The table holds a byte per pixel of the metatile, row by row. */
	return CURVE_HEIGHT_FIXED(
		samples[TerrainSampleIndexTable[pixelX + (pixelY << 4)]]);
}

/* The same, with the table mirrored left to right. */
FIXED fdTerrainGetSampleTableMirroredHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	/* Row y read from its right end: (y << 4) + 15 - x. */
	return CURVE_HEIGHT_FIXED(
		samples[TerrainSampleIndexTable[((pixelY << 4) - pixelX) +
			CURVE_FULL_STEPS - 1]]);
}

/* Flat, as fdTerrainGetFlatHeight, for TERRAIN_FLAT_B. */
FIXED fdTerrainGetFlatBHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	return CURVE_HEIGHT_FIXED(samples[0]);
}

/* Flat, as fdTerrainGetFlatHeight, for TERRAIN_FLAT_C. */
FIXED fdTerrainGetFlatCHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY)
{
	const Sint8 *samples = (const Sint8 *)heightBytes;
	return CURVE_HEIGHT_FIXED(samples[0]);
}

#undef TERRAIN_DEFINE_SLOPE
#undef TERRAIN_DEFINE_STEP
#undef CURVE_HEIGHT_FIXED

/* Returns the ground height of a layer at (x, y) in 16.16 pixels: the height
   function of the metatile's terrain kind applied to its samples. y is the
   map's down axis, an actor's z. */
FIXED fdMapGridGetTerrainHeight(Sint32 layer, FIXED x, FIXED y)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	MAP_GRID_METATILE	  *metatile;
	Uint8				  *heightBytes;

	x = FIXEDtoINT(x);
	y = FIXEDtoINT(y);
	/* Before the map state exists, the grid is read from its start. */
	metatile = (MAP_GRID_METATILE *)Ram_MapGridMetatileBuffer;
	if(grid != NULL) {
		metatile = grid->layers[layer & 3].metatiles;
	}
	/* Metatiles are 16 pixels, 128 to a row. */
	metatile += x / MAPGRID_METATILE_PIXELS +
		(y / MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE;
	heightBytes = Ram_MapGridShapes + metatile->shape * 4;
	/* UB: read the kind byte before passing the remaining samples. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
	{
		Uint32 kind = *heightBytes++ & MAPGRID_SHAPE_KIND_MASK;

		return MapGridTerrainHeightFunctions[kind](heightBytes,
			x & (MAPGRID_METATILE_PIXELS - 1),
			y & (MAPGRID_METATILE_PIXELS - 1));
	}
#else
	return MapGridTerrainHeightFunctions[*heightBytes++ &
		MAPGRID_SHAPE_KIND_MASK](heightBytes, x & (MAPGRID_METATILE_PIXELS - 1),
		y & (MAPGRID_METATILE_PIXELS - 1));
#endif
}

/* Returns the terrain kind (TERRAIN_KIND) of a layer's metatile at (x, y) in
   16.16 pixels. */
Sint32 fdMapGridGetTerrainKindAt(Sint32 layer, FIXED x, FIXED y)
{
	MAP_GRID_SCROLL_STATE *gridState = MapGridState[0];
	MAP_GRID_METATILE	  *metatiles;
	Sint32				   metatileX;
	Sint32				   metatileY;
	Uint32				   shape;

	x = FIXEDtoINT(x);
	y = FIXEDtoINT(y);
	metatiles = (MAP_GRID_METATILE *)MapGridMetatileBuffer;
	if(gridState != 0) {
		metatiles = gridState->layers[layer & 3].metatiles;
	}
	metatileX = x / MAPGRID_METATILE_PIXELS;
	metatileY = y / MAPGRID_METATILE_PIXELS;
	metatiles += metatileX + metatileY * MAPGRID_SIZE;
	/* The kind byte of the metatile's four-byte shape. */
	shape = metatiles->shape;
	return *(Ram_MapGridShapes + shape * 4) & MAPGRID_SHAPE_KIND_MASK;
}

/* Returns the metatile behavior of a layer's metatile at (x, y) in 16.16
   pixels. */
Sint32 fdMapGridGetMetatileBehaviorAt(Sint32 layer, FIXED x, FIXED y)
{
	MAP_GRID_SCROLL_STATE *gridState;
	MAP_GRID_METATILE	  *metatile;

	gridState = MapGridState[0];
	x = FIELD_METATILE(x);
	y = FIELD_METATILE(y);
	/* Before the map state exists, the grid is read from its start. */
	metatile = (MAP_GRID_METATILE *)MapGridMetatileBuffer;
	if(gridState != NULL) {
		metatile = gridState->layers[layer & 3].metatiles;
	}
	metatile += x + y * MAPGRID_SIZE;
	return metatile->behavior;
}

/* Sets the metatile behavior of a layer's metatile at (x, y) in 16.16 pixels.
   Writes the metatile in the map's grid; does nothing before the map is set
   up. */
void fdMapGridSetMetatileBehaviorAt(
	Uint32 layer, FIXED x, FIXED y, Uint32 behavior)
{
	MAP_GRID_SCROLL_STATE *gridState = MapGridState[0];

	x = FIELD_METATILE(x);
	y = FIELD_METATILE(y);
	/* Without the map state there is no layer to write to. */
	if(gridState != NULL) {
		MAP_GRID_METATILE *metatile = gridState->layers[layer & 3].metatiles;

		metatile += x + y * MAPGRID_SIZE;
		metatile->behavior = behavior;
	}
}

/* Unused: returns the top two bits of the metatile tiles halfword of the
   metatile at pixel (x, y). */
UNUSED Sint32 fdMapGridGetMetatileHighFlags(Sint32 x, Sint32 y)
{
	Sint32			   metatileX = x / MAPGRID_METATILE_PIXELS;
	Sint32			   metatileY = y / MAPGRID_METATILE_PIXELS;
	MAP_GRID_METATILE *metatile = (MAP_GRID_METATILE *)MapGridMetatileBuffer +
		(metatileX + metatileY * MAPGRID_SIZE);

	/* The high byte of the metatile tiles halfword; its top two bits are
	   flags. */
	return ((Uint8 *)&metatile->metatileTilesAndFlg)[1] >> 6;
}

/* Returns whether an actor can step to position, an COLLISION: whether the
   metatile there is a wall, or the ground too high or too low for a step from
   the actor's. */
Sint32 fdMapGridCheckCollision(ACTOR *actor, FIELD_POS *pos)
{
	MAP_GRID_SCROLL_STATE *gridState;
	Sint32				   pixelX;
	Sint32				   pixelZ;
	MAP_GRID_METATILE	  *metatile;
	Uint8				  *heightBytes;
	Sint32				   tgtGround;
	Sint32				   stepHeight;

	pixelX = ((FIELD_COORDINATE *)&pos->x)->part[0].integer;
	pixelZ = ((FIELD_COORDINATE *)&pos->z)->part[0].integer;
	gridState = MapGridState[0];
	if(gridState == NULL)
		return COLLISION_NONE;
	/* The three layers have their windows into the grid; any other layer reads
	   the grid from its start. */
	if(actor->layer <= MAPGRID_LAYER_OVERHEAD)
		metatile = gridState->layers[actor->layer].metatiles;
	else
		metatile = (MAP_GRID_METATILE *)Ram_MapGridMetatileBuffer;
	metatile += pixelX / MAPGRID_METATILE_PIXELS +
		(pixelZ / MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE;
	/* A wall blocks whatever the heights. */
	if(metatile->behavior == METATILE_BEHAVIOR_WALL)
		return COLLISION_IMPASSABLE;
	heightBytes = Ram_MapGridShapes + metatile->shape * 4;
	/* UB: read the kind byte before passing the remaining samples. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
	{
		Uint32 kind = *heightBytes++ & MAPGRID_SHAPE_KIND_MASK;

		tgtGround = MapGridTerrainHeightFunctions[kind](heightBytes,
			pixelX & (MAPGRID_METATILE_PIXELS - 1),
			pixelZ & (MAPGRID_METATILE_PIXELS - 1));
	}
#else
	tgtGround =
		MapGridTerrainHeightFunctions[*heightBytes++ & MAPGRID_SHAPE_KIND_MASK](
			heightBytes, pixelX & (MAPGRID_METATILE_PIXELS - 1),
			pixelZ & (MAPGRID_METATILE_PIXELS - 1));
#endif
	/* Up to 8 pixels up or 12 down is a step the actor can take. */
	stepHeight = tgtGround - actor->groundHeight;
	if(stepHeight > toFIXED(8))
		return COLLISION_TOO_HIGH;
	if(stepHeight < -toFIXED(12))
		return COLLISION_TOO_LOW;
	return COLLISION_NONE;
}

/* Returns -1 when the metatile under position (its x, and y less its height) in
   MAPGRID_LAYER_OVERHEAD is a wall (METATILE_BEHAVIOR_WALL), else 0; also 0
   before the map state exists. */
Sint32 fdMapGridIsWallAt(WORLD_POS *pos)
{
	Sint32				   pixelX;
	Sint32				   pixelY;
	Sint32				   metatileX;
	Sint32				   metatileY;
	MAP_GRID_SCROLL_STATE *gridState;
	MAP_GRID_METATILE	  *metatile;

	pixelX = pos->x / toFIXED(1);
	/* The point on the ground below the position. */
	pixelY = (pos->y - pos->height) / toFIXED(1);
	gridState = MapGridState[0];
	if(gridState == NULL)
		return 0;
	/* Always the overhead layer, the one on BG1. */
	metatile = gridState->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
	metatileX = pixelX / MAPGRID_METATILE_PIXELS;
	metatileY = pixelY / MAPGRID_METATILE_PIXELS;
	metatile += metatileX + metatileY * MAPGRID_SIZE;
	/* 1 - 1 for open ground, 0 - 1 for a wall: nonzero means walled. */
	return (metatile->behavior != METATILE_BEHAVIOR_WALL) - 1;
}

/* The world map's collision: each tile of a terrain layer's map has a tile of
   4-bit collision values, 8 bytes, two values a byte. The lower layer's map is
   in screen block 8, the upper's in screen block 10, and the upper's collision
   tiles follow the lower's 256 in the buffer. */
#define WORLD_MAP_LOWER_COLLISION_MAP	BG_SCREEN_BLOCK(8)
#define WORLD_MAP_UPPER_COLLISION_MAP	BG_SCREEN_BLOCK(10)
#define WORLD_MAP_COLLISION_TILE_BYTES	8
#define WORLD_MAP_UPPER_COLLISION_TILES (256 * WORLD_MAP_COLLISION_TILE_BYTES)
#define WORLD_MAP_COLLISION_BITS		4
#define WORLD_MAP_COLLISION_MASK		((1 << WORLD_MAP_COLLISION_BITS) - 1)

/* A block word's bit 31, in its top byte: the second bank of battle
   environments. */
#define WORLD_MAP_BLOCK_SECOND_BANK	 (1 << (31 - 24))
/* A block word's encounter region, its bits 24 to 30. */
#define WORLD_MAP_BLOCK_REGION(word) (((word) << 1) >> 25)
/* The battle environments of WorldMapBattleEnvironments come in banks of one
   for each collision value. */
#define WORLD_MAP_ENVIRONMENT_BANK	 16

/* Returns the world map's collision value (1-15) at position: from the upper
   collision layer where it has one, else the lower, else
   WORLD_MAP_COLLISION_DEFAULT. */
Sint32 fdWorldMapGetCollision(WORLD_POS *pos)
{
	Sint32 halfPixelX = FIXEDtoINT(pos->x) >> 1;
	Sint32 y = pos->y;
	Sint32 halfPixelY = FIXEDtoINT(y) >> 1;
	Uint32 collisionMetatile;
	Uint32 pixelOfs;
	Uint8  collisionTile;
	Uint8  packedCollisions;
	Uint32 result;

	/* Coordinates in 2-pixel units. */
	collisionMetatile =
		(((halfPixelY / 8) & 63) << 6) + ((halfPixelX / 8) & 63);
	pixelOfs = (((halfPixelY / 2) & 3) << 1) + ((halfPixelX / 4) & 1);

	/* The upper layer first. */
	collisionTile = WORLD_MAP_UPPER_COLLISION_MAP[collisionMetatile];
	packedCollisions = *(Ram_MapGridShapes + WORLD_MAP_UPPER_COLLISION_TILES +
		collisionTile * WORLD_MAP_COLLISION_TILE_BYTES + pixelOfs);
	if(packedCollisions != 0) {
		if(halfPixelX & 2)
			result = packedCollisions >> WORLD_MAP_COLLISION_BITS;
		else
			result = packedCollisions & WORLD_MAP_COLLISION_MASK;
		if(result != 0)
			return result;
	}

	/* Then the lower layer. */
	collisionTile = WORLD_MAP_LOWER_COLLISION_MAP[collisionMetatile];
	packedCollisions = *(Ram_MapGridShapes +
		collisionTile * WORLD_MAP_COLLISION_TILE_BYTES + pixelOfs);
	if(packedCollisions != 0) {
		if(halfPixelX & 2)
			result = packedCollisions >> WORLD_MAP_COLLISION_BITS;
		else
			result = packedCollisions & WORLD_MAP_COLLISION_MASK;
		if(result != 0)
			return result;
	}

	/* Neither layer has a value: the default, inside the walkable range. */
	return WORLD_MAP_COLLISION_DEFAULT;
}

/* Returns 0 when the world map at position can be walked on (collision
   WORLD_MAP_COLLISION_WALKABLE_FIRST to WORLD_MAP_COLLISION_WALKABLE_LAST),
   else -1. */
Sint32 fdWorldMapIsWalkable(Sint32 unused, WORLD_POS *pos)
{
	/* One unsigned compare tests the walkable range. */
	if((fdWorldMapGetCollision(pos) - WORLD_MAP_COLLISION_WALKABLE_FIRST) <=
		(Uint32)(WORLD_MAP_COLLISION_WALKABLE_LAST -
			WORLD_MAP_COLLISION_WALKABLE_FIRST)) {
		return 0;
	}
	return -1;
}

/* Returns the world map's battle environment at position from
   WorldMapBattleEnvironments, and stores the block's encounter region in
   *region. */
Sint32 fdWorldMapGetBattleEnvironment(WORLD_POS *pos, Sint32 *region)
{
	Sint32	collisionKind = fdWorldMapGetCollision(pos);
	Sint32	x = pos->x;
	Sint32	y;
	Sint32	environmentBank = 0;
	Uint32	blockX;
	Uint32	idx;
	Uint32 *blockWord;

	blockX =
		(x / toFIXED(WORLD_MAP_BLOCK_PIXELS)) & (WORLD_MAP_BLOCKS_ACROSS - 1);
	y = pos->y;

	idx = blockX +
		((y / toFIXED(WORLD_MAP_BLOCK_PIXELS)) &
			(WORLD_MAP_BLOCKS_ACROSS - 1)) *
			WORLD_MAP_BLOCKS_ACROSS;
	blockWord = MetatileTiles + idx;
	/* Bit 31 of the block word selects the second bank; the snowfield the
	   third, whatever bit 31 says. */
	if(((Uint8 *)blockWord)[3] & WORLD_MAP_BLOCK_SECOND_BANK)
		environmentBank = WORLD_MAP_ENVIRONMENT_BANK;

	*region = WORLD_MAP_BLOCK_REGION(*blockWord);
	if(*region == WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD)
		environmentBank = 2 * WORLD_MAP_ENVIRONMENT_BANK;

	return WorldMapBattleEnvironments[environmentBank + collisionKind];
}

#undef WORLD_MAP_LOWER_COLLISION_MAP
#undef WORLD_MAP_UPPER_COLLISION_MAP
#undef WORLD_MAP_COLLISION_TILE_BYTES
#undef WORLD_MAP_UPPER_COLLISION_TILES
#undef WORLD_MAP_COLLISION_BITS
#undef WORLD_MAP_COLLISION_MASK
#undef WORLD_MAP_BLOCK_SECOND_BANK
#undef WORLD_MAP_BLOCK_REGION
#undef WORLD_MAP_ENVIRONMENT_BANK
