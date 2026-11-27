/* The screen shake, and two parts of the world map's view: its still image
   drawn from its tile maps, and its ground tilted away from the camera, worked
   out a screen line at a time. Part of field.c. */

#include "field_internal.h"

/* The longest fdMapGridWaitForShakeToSettle waits, five seconds. */
#define MAPGRID_SHAKE_SETTLE_FRAMES		   300
/* The largest shake that counts as settled, just under 1/256 pixel. */
#define MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE (toFIXED(1) / 256 - 1)
/* One of the world map's two tile maps of one-byte tiles, BG3's and then BG2's,
   as fdWorldMapStartStillImage copies them into DecodeBuffer. */
#define WORLD_MAP_TILE_MAP_BYTES		   (64 * 64)

/* Changes the shake without restarting an axis whose argument is negative.
   Amplitudes are 16.16 pixels and decay is a 16.16 multiplier per frame. */
void fdMapGridSetScreenShake(FIXED shakeX, FIXED shakeY, FIXED shakeDecay)
{
	MAP_GRID_SCROLL_STATE *grid;

	/* A negative argument leaves that value alone, so a script can change the
	   decay without restarting the shake, or start one axis only. */
	grid = MapGridState[0];
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

/* Waits until both shake amplitudes have settled, at most
   MAPGRID_SHAKE_SETTLE_FRAMES for a shake whose decay never settles. */
void fdMapGridWaitForShakeToSettle(void)
{
	MAP_GRID_SCROLL_STATE *grid;
	Sint32				   waitedFrames;

	grid = MapGridState[0];
	/* A decay of 1.0 or more never settles, so the limit ends the wait. */
	for(waitedFrames = 0; waitedFrames < MAPGRID_SHAKE_SETTLE_FRAMES &&
		(grid->shakeX > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE ||
			grid->shakeY > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE);
		waitedFrames++) {
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
	const Uint8 *maps;

	/* The third tile set and the second tile map are BG2's. */
	maps = DecodeBuffer;
	RAM_CODE_LOAD_SLOT(
		HEAP_SLOT_RAM_CODE_TEMP, fdTileMapDrawRows, fdTileMapDrawRowsCodeSize);
	((TILE_MAP_ROW_DRAWER)HeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(lines, pixels,
		BgTileBuffer + 2 * WORLD_MAP_TILE_SET_BYTES,
		maps + WORLD_MAP_TILE_MAP_BYTES);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);
}

/* Works out each screen line of the world map's tilted ground: how much the
   ground is scaled on that line and how far along it the line lies from the
   followed point, for a camera pitched by pitchSlope. The lines above the
   horizon, sky, get zeros. */
void fdWorldMapBuildScanlineTable(
	FIXED pitchSlope, Sint32 *position, WORLD_MAP_SCANLINE *scanline)
{
	Sint32 groundPoint[3];
	Sint32 cameraPoint[3];
	FIXED  horizon;
	FIXED  focus;
	Sint32 line;
	FIXED  slope;
	FIXED  difference;
	FIXED  scale;
	FIXED  horizontalSquared;
	FIXED  verticalSquared;
	FIXED  halfScale;
	FIXED  rayHeight;
	FIXED  groundDistance;
	Sint32 vertical;
	Sint32 groundHeight = 0;
	Sint32 horizontal;

	/* The followed point on the ground (height 0), in camera space; horizon is
	   its height less its depth times the pitch slope. */
	SET_VECTOR(groundPoint, position[0], groundHeight, position[2]);
	syCalcPoint(groundPoint, cameraPoint);
	horizon = cameraPoint[1] - syMulFX(cameraPoint[2], pitchSlope);
	focus = -ProjectionWork.focal;
	for(line = 0; line < DISPLAY_HEIGHT; line++) {
		/* The line's ray slope: its offset from the projection's centre line
		   over the focal length. */
		slope = syDivFX(focus, toFIXED(ProjectionWork.centerY - line));
		/* A ray parallel to the ground would divide by zero. */
		difference = slope - pitchSlope;
		if(difference == 0)
			difference = 1;
		scale = syDivFX(difference, horizon);
		/* A negative scale is a line below the horizon, whose ray meets the
		   ground; the lines above it are sky and get zeros. */
		if(scale < 0) {
			/* The width scale is half the line's scale over the focal length,
			   and the depth is stored halved. */
			halfScale = syMulFX(-scale, toFIXED(1) / 2);
			scanline->scale = syDivFX(ProjectionWork.focal, halfScale);
			rayHeight = syMulFX(scale, slope);
			/* The distance along the ground from the followed point: the root
			   of the two squared terms, taken in sixteenths (the root of a
			   16.16 square comes back in 8.8), with the sign of the vertical
			   term. */
			horizontal = (cameraPoint[2] - scale) >> 4;
			vertical = (rayHeight - cameraPoint[1]) >> 4;
			horizontalSquared = syMulFX(horizontal, horizontal);
			verticalSquared = syMulFX(vertical, vertical);
			groundDistance = sySquartIWRAM(horizontalSquared + verticalSquared)
				<< (8 + 4);
			if(vertical < 0)
				groundDistance = -groundDistance;
			scanline->depth = syMulFX(groundDistance, toFIXED(1) / 2);
		} else {
			scanline->scale = 0;
			scanline->depth = 0;
		}
		scanline->ofsX = 0;
		scanline->ofsY = 0;
		scanline++;
	}
}

#undef MAPGRID_SHAKE_SETTLE_FRAMES
#undef MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE
#undef WORLD_MAP_TILE_MAP_BYTES
