// Part of field.c, the field object, compiled only through it.
// Screen shake, the copied BG row drawer and the ground plane scanlines.

#include "../field_internal.h"

// Shakes the screen: sets the map's x and y shake amplitude and its decay
// (16.16), leaving any negative value unchanged.
void Map_SetScreenShake(s32 shakeX, s32 shakeY, s32 shakeDecay) {
  struct MapScrollWork *work;

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

// Waits until the screen shake has died down (both amplitudes under 1/256
// pixel), at most 300 frames, then stops it.
void Map_WaitForShakeToSettle(void) {
  struct MapScrollWork *work;
  s32 waitedFrames;

  work = gMapWork[0];
  waitedFrames = 0;
  while (waitedFrames < 300 && (work->shakeX > 255 || work->shakeY > 255)) {
    WaitFrames(1);
    waitedFrames++;
  }
  work->shakeDecay = 0;
}

// Copies TileMap_DrawRows (ARM code) to the heap and runs it once: it draws
// the rows that lines describes (32 bytes a row) into pixels, from the tiles
// in the BG tile buffer's second half and the map in the decode buffer.
void TileMap_DrawRowsFromRam(s32 lines, s32 pixels) {
  u8 *base;

  base = gDecodeBuffer;
  RamCode_LoadSlot(49, TileMap_DrawRows, TileMap_DrawRowsCodeSize);
  /* FAKEMATCH(one-pass): its loop notes keep gBgTileBuffer's address load ahead
   * of lines' move into r0, as native; as the bare call, 1 of 39 instructions
   * differs in tbs-en. */
  do {
    ((void (*)(s32, s32, void *, void *))gWorkSlot[49])(
        lines, pixels, gBgTileBuffer + 0x4000, base + 0x1000);
  } while (0);
  Runtime_ReleaseHeapBlock(49);
}

// Builds the world map ground plane's per-scanline terms for the camera's
// pitch (depth is its cotangent): for each of the 160 lines, where its ray
// meets the ground, the line's width scale and its distance, or zeros above
// the horizon. Transform_UpdateVertices turns these into the affine
// parameters each frame.
void WorldMap_BuildScanlineTable(s32 depth, s32 *position,
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

  zero = 0;
  SET_VECTOR(groundPoint, position[0], zero, position[2]);
  Iwram_TransformVector(groundPoint, cameraPoint);
  cameraCoordinates = cameraPoint;
  horizon = cameraCoordinates[1] - Iwram_MulQ16(cameraCoordinates[2], depth);
  focus = -gProjection.focal;
  for (line = 0; line < 160; line++) {
    // Lines below the screen centre have a negative displacement.
    distance = Iwram_RatioMulQ14(focus, (gProjection.centerY - line) * 65536);
    difference = distance - depth;
    if (difference == 0)
      difference = 1;
    scale = Iwram_RatioMulQ14(difference, horizon);
    if (scale < 0) {
      diagonal = Iwram_MulQ16(-scale, 0x8000);
      scanline->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
      diagonal = Iwram_MulQ16(scale, distance);
      horizontal = (cameraCoordinates[2] - scale) >> 4;
      vertical = (diagonal - cameraCoordinates[1]) >> 4;
      horizontalSquared = Iwram_MulQ16(horizontal, horizontal);
      verticalSquared = Iwram_MulQ16(vertical, vertical);
      verticalSquared = Iwram_Sqrt(horizontalSquared + verticalSquared) << 12;
      if (vertical < 0)
        verticalSquared = -verticalSquared;
      scanline->depth = Iwram_MulQ16(verticalSquared, 0x8000);
    } else {
      scanline->scale = 0;
      scanline->depth = 0;
    }
    scanline->offsetX = 0;
    scanline->offsetY = 0;
    scanline++;
  }
}
