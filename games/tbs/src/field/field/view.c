/// @file
/// Screen shake, the copied BG row drawer and the ground plane scanlines.
// Part of field.c.

#include "../field_internal.h"

#define MAP_SHAKE_SETTLE_FRAMES 300
#define MAP_SHAKE_SETTLE_MAX_AMPLITUDE (Q16_ONE / 256 - 1)
#define TILEMAP_TILE_BANK_BYTES (256 * (8 * 8))
#define TILEMAP_MAP_BANK_BYTES (64 * 64)

/// Changes the shake without restarting an axis whose argument is negative.
/// Amplitudes are 16.16 pixels and decay is a 16.16 multiplier per frame.
/// Scene commands reach it through Map_SetShake and Map_SetScreenShakeFar.
void Map_SetScreenShake(s32 shakeX, s32 shakeY, s32 shakeDecay) {
  struct MapScrollWork *work;

  // A negative argument leaves that value alone, so a scene can change the
  // decay without restarting the shake, or start one axis only. The view
  // updates read the same three words on the world map (PerspectiveWork).
  work = gMapWork[0];
  if (shakeX >= 0) {
    work->shakeX = shakeX;
  }
  if (shakeY >= 0) {
    work->shakeY = shakeY;
  }
  if (shakeDecay >= 0) {
    work->shakeDecay = shakeDecay;
  }
}

/// Waits until both shake amplitudes are below 1/256 pixel, with a 300-frame
/// limit for a shake whose decay never settles. Scene imports reach it as
/// Map_WaitForShakeToSettleFar; it stops further shake after the wait.
void Map_WaitForShakeToSettle(void) {
  struct MapScrollWork *work;
  s32 waitedFrames;

  work = gMapWork[0];
  waitedFrames = 0;
  // 300 frames is five seconds; 255 in 16.16 is just under 1/256 pixel.
  // A decay of 1.0 or more never settles, so the limit ends the wait.
  while (waitedFrames < MAP_SHAKE_SETTLE_FRAMES &&
         (work->shakeX > MAP_SHAKE_SETTLE_MAX_AMPLITUDE ||
          work->shakeY > MAP_SHAKE_SETTLE_MAX_AMPLITUDE)) {
    WaitFrames(1);
    waitedFrames++;
  }
  // A zero decay makes the next view update's shake its last.
  work->shakeDecay = 0;
}

/// The copied ARM kernel reads the BG2 affine row from each 32-byte pair and
/// writes 240 by 160 pixels as 256-colour tiles.
typedef void (*TileMapRowDrawer)(const struct MapAffinePair *lines, u8 *pixels,
                                 const u8 *tiles, const u8 *map);

/// Draws the current affine page into the world map's still-view pixels.
/// WorldMap_StartStillView supplies 160 pairs and a 240 by 160 byte output.
/// The ARM kernel borrows HEAP_SLOT_TEMP_RENDERER for this synchronous call;
/// it is released before returning, while both graphics buffers stay owned
/// by the world map.
void TileMap_DrawRowsFromRam(const struct MapAffinePair *lines, u8 *pixels) {
  const u8 *base;

  // The second tile and map banks are BG2. The kernel falls back to BG3's
  // preceding banks when a BG2 pixel is transparent.
  base = gDecodeBuffer;
  RamCode_LoadSlot(HEAP_SLOT_TEMP_RENDERER, TileMap_DrawRows,
                   TileMap_DrawRowsCodeSize);
  /* FAKEMATCH(one-pass): its loop notes keep gBgTileBuffer's address load ahead
   * of lines' move into r0, as native; as the bare call, 1 of 39 instructions
   * differs in tbs-en. */
  do {
    ((TileMapRowDrawer)gWorkSlot[HEAP_SLOT_TEMP_RENDERER])(
        lines, pixels, gBgTileBuffer + TILEMAP_TILE_BANK_BYTES,
        base + TILEMAP_MAP_BANK_BYTES);
  } while (0);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_TEMP_RENDERER);
}

/// Rebuilds the ground plane's scanline terms when the world map pitch changes.
/// Map_InitializePerspectiveScene and WorldMap_UpdateView supply the pitch's
/// sine/cosine ratio and the followed position, both in 16.16. Each screen line
/// gets a width scale and signed ground distance, or zeros above the horizon;
/// Transform_UpdateVertices reads them to build the frame's affine page.
void WorldMap_BuildScanlineTable(s32 pitchSlope, s32 *position,
                                 struct MapScanline *scanline) {
  s32 groundPoint[3];
  s32 cameraPoint[3];
  s32 horizon;
  s32 focus;
  s32 *cameraCoordinates;
  s32 line;
  s32 distance;
  s32 difference;
  s32 scale;
  s32 horizontalSquared;
  s32 verticalSquared;
  s32 diagonal;
  s32 vertical;
  s32 zero;
  s32 horizontal;

  // The followed point on the ground (height 0), in camera space; horizon is
  // its height less its depth times the pitch slope.
  zero = 0;
  SET_VECTOR(groundPoint, position[0], zero, position[2]);
  Iwram_TransformVector(groundPoint, cameraPoint);
  cameraCoordinates = cameraPoint;
  horizon =
      cameraCoordinates[1] - Iwram_MulQ16(cameraCoordinates[2], pitchSlope);
  focus = -gProjection.focal;
  for (line = 0; line < FIELD_VIEW_HEIGHT / Q16_ONE; line++) {
    // The line's ray slope: its offset from the projection's centre line
    // over the focal length. Lines below the screen centre have a negative
    // displacement.
    distance = Iwram_RatioMulQ14(focus, (gProjection.centerY - line) * Q16_ONE);
    // A ray parallel to the ground would divide by zero.
    difference = distance - pitchSlope;
    if (difference == 0)
      difference = 1;
    scale = Iwram_RatioMulQ14(difference, horizon);
    // A negative scale is a line below the horizon, whose ray meets the
    // ground; the lines above it are sky and get zeros.
    if (scale < 0) {
      // MulQ16 by 0x8000 halves: the width scale is half the line's scale
      // over the focal length, and the distance below is stored halved.
      diagonal = Iwram_MulQ16(-scale, Q16_ONE / 2);
      scanline->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
      diagonal = Iwram_MulQ16(scale, distance);
      // The distance is the root of the two squared terms. Both are taken
      // >> 4 first so the squares stay in range, which leaves the root
      // >> 12; << 12 puts it back in 16.16.
      horizontal = (cameraCoordinates[2] - scale) >> 4;
      vertical = (diagonal - cameraCoordinates[1]) >> 4;
      horizontalSquared = Iwram_MulQ16(horizontal, horizontal);
      verticalSquared = Iwram_MulQ16(vertical, vertical);
      verticalSquared = Iwram_Sqrt(horizontalSquared + verticalSquared) << 12;
      // The ground distance takes the sign of the vertical term.
      if (vertical < 0)
        verticalSquared = -verticalSquared;
      scanline->depth = Iwram_MulQ16(verticalSquared, Q16_ONE / 2);
    } else {
      scanline->scale = 0;
      scanline->depth = 0;
    }
    scanline->offsetX = 0;
    scanline->offsetY = 0;
    scanline++;
  }
}

#undef MAP_SHAKE_SETTLE_FRAMES
#undef MAP_SHAKE_SETTLE_MAX_AMPLITUDE
#undef TILEMAP_TILE_BANK_BYTES
#undef TILEMAP_MAP_BANK_BYTES
