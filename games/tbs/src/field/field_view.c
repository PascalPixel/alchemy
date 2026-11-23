/// @file
/// The screen shake, and two parts of the world map's view: its still picture
/// drawn from its tile maps, and its ground tilted away from the camera,
/// worked out a screen line at a time.
// Part of field.c.

#include "field_internal.h"

/// The longest MapGrid_WaitForShakeToSettle waits, five seconds.
#define MAPGRID_SHAKE_SETTLE_FRAMES 300
/// The largest shake that counts as settled, just under 1/256 pixel.
#define MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE (toFIXED(1) / 256 - 1)
/// One of the world map's two tile maps of one-byte tiles, BG3's and then
/// BG2's, as WorldMap_StartStillPicture copies them into gDecodeBuffer.
#define WORLD_MAP_TILE_MAP_BYTES (64 * 64)

/// Changes the shake without restarting an axis whose argument is negative.
/// Amplitudes are 16.16 pixels and decay is a 16.16 multiplier per frame.
void MapGrid_SetScreenShake(FIXED shakeX, FIXED shakeY, FIXED shakeDecay) {
  struct MapGridScrollWork *grid;

  // A negative argument leaves that value alone, so an event can change the
  // decay without restarting the shake, or start one axis only.
  grid = gMapGridWork[0];
  if (shakeX >= 0) {
    grid->shakeX = shakeX;
  }
  if (shakeY >= 0) {
    grid->shakeY = shakeY;
  }
  if (shakeDecay >= 0) {
    grid->shakeDecay = shakeDecay;
  }
}

/// Waits until both shake amplitudes have settled, at most
/// MAPGRID_SHAKE_SETTLE_FRAMES for a shake whose decay never settles.
void MapGrid_WaitForShakeToSettle(void) {
  struct MapGridScrollWork *grid;
  s32 waitedFrames;

  grid = gMapGridWork[0];
  // A decay of 1.0 or more never settles, so the limit ends the wait.
  for (waitedFrames = 0; waitedFrames < MAPGRID_SHAKE_SETTLE_FRAMES &&
                         (grid->shakeX > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE ||
                          grid->shakeY > MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE);
       waitedFrames++) {
    wait(1);
  }
  // A zero decay makes the next view update's shake its last.
  grid->shakeDecay = 0;
}

/// The ARM routine copied to RAM reads the BG2 affine row from each 32-byte
/// pair and writes 240 by 160 pixels as 256-colour tiles; it returns the
/// address past the last pair, where its row cursor stops.
typedef const struct WorldMapAffinePair *(*TileMapRowDrawer)(
    const struct WorldMapAffinePair *lines, u8 *pixels, const u8 *tiles,
    const u8 *map);

/// Draws the current affine page into the world map's still-picture pixels.
/// WorldMap_StartStillPicture supplies 160 pairs and a 240 by 160 byte output.
void TileMap_DrawRowsFromRam(const struct WorldMapAffinePair *lines,
                             u8 *pixels) {
  const u8 *maps;

  // The third tile set and the second tile map are BG2's.
  maps = gDecodeBuffer;
  RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, TileMap_DrawRows,
                   TileMap_DrawRowsCodeSize);
  ((TileMapRowDrawer)gHeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(
      lines, pixels, gBgTileBuffer + 2 * WORLD_MAP_TILE_SET_BYTES,
      maps + WORLD_MAP_TILE_MAP_BYTES);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_TEMP);
}

/// Works out each screen line of the world map's tilted ground: how much the
/// ground is scaled on that line and how far along it the line lies from the
/// followed point, for a camera pitched by pitchSlope. The lines above the
/// horizon, sky, get zeros.
void WorldMap_BuildScanlineTable(FIXED pitchSlope, s32 *position,
                                 struct WorldMapScanline *scanline) {
  s32 groundPoint[3];
  s32 cameraPoint[3];
  FIXED horizon;
  FIXED focus;
  s32 line;
  FIXED slope;
  FIXED difference;
  FIXED scale;
  FIXED horizontalSquared;
  FIXED verticalSquared;
  FIXED halfScale;
  FIXED rayHeight;
  FIXED groundDistance;
  s32 vertical;
  s32 groundHeight = 0;
  s32 horizontal;

  // The followed point on the ground (height 0), in camera space; horizon is
  // its height less its depth times the pitch slope.
  SET_VECTOR(groundPoint, position[0], groundHeight, position[2]);
  Iwram_TransformVector(groundPoint, cameraPoint);
  horizon = cameraPoint[1] - Iwram_MulFX(cameraPoint[2], pitchSlope);
  focus = -gProjection.focal;
  for (line = 0; line < DISPLAY_HEIGHT; line++) {
    // The line's ray slope: its offset from the projection's centre line over
    // the focal length.
    slope = Iwram_RatioMulQ14(focus, toFIXED(gProjection.centerY - line));
    // A ray parallel to the ground would divide by zero.
    difference = slope - pitchSlope;
    if (difference == 0)
      difference = 1;
    scale = Iwram_RatioMulQ14(difference, horizon);
    // A negative scale is a line below the horizon, whose ray meets the
    // ground; the lines above it are sky and get zeros.
    if (scale < 0) {
      // The width scale is half the line's scale over the focal length, and
      // the depth is stored halved.
      halfScale = Iwram_MulFX(-scale, FIXED_ONE / 2);
      scanline->scale = Iwram_RatioMulQ14(gProjection.focal, halfScale);
      rayHeight = Iwram_MulFX(scale, slope);
      // The distance along the ground from the followed point: the root of
      // the two squared terms, taken in sixteenths (the root of a 16.16
      // square comes back in 8.8), with the sign of the vertical term.
      horizontal = (cameraPoint[2] - scale) >> 4;
      vertical = (rayHeight - cameraPoint[1]) >> 4;
      horizontalSquared = Iwram_MulFX(horizontal, horizontal);
      verticalSquared = Iwram_MulFX(vertical, vertical);
      groundDistance = Iwram_Sqrt(horizontalSquared + verticalSquared)
                       << (8 + 4);
      if (vertical < 0)
        groundDistance = -groundDistance;
      scanline->depth = Iwram_MulFX(groundDistance, FIXED_ONE / 2);
    } else {
      scanline->scale = 0;
      scanline->depth = 0;
    }
    scanline->offsetX = 0;
    scanline->offsetY = 0;
    scanline++;
  }
}

#undef MAPGRID_SHAKE_SETTLE_FRAMES
#undef MAPGRID_SHAKE_SETTLE_MAX_AMPLITUDE
#undef WORLD_MAP_TILE_MAP_BYTES
