// Draft, differ 70 of 132: builds the perspective map's 160 scanline
// terms for the camera pitch (depth is its cotangent), ☀️'s
// WorldMap_BuildScanlineTable (games/tbs/src/field/field.c) word for word
// over ⚓️'s projection record. Left is register choice: the native keeps
// the line counter in fp and the camera point's address in a stack slot
// where this draft swaps them, which swaps r5 and r6 through the sqrt
// block.
#include "types.h"
#include "macros.h"
#include "iwram_call.h"

struct MapScanline {
  s32 scale, depth;
  s32 offsetX, offsetY;
  s16 pa, pc;
};

struct Projection {
  s32 focal;
  s32 unk04[3];
  s32 centerY;
};

extern struct Projection gCameraSceneParameters;

void Func_0802dd70(s32 depth, s32 *position, struct MapScanline *scanline) {
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
  focus = -gCameraSceneParameters.focal;
  for (line = 0; line < 160; line++) {
    distance = Iwram_RatioMulQ14(
        focus, (gCameraSceneParameters.centerY - line) * 65536);
    difference = distance - depth;
    if (difference == 0)
      difference = 1;
    scale = Iwram_RatioMulQ14(difference, horizon);
    if (scale < 0) {
      diagonal = Iwram_MulQ16(-scale, 0x8000);
      scanline->scale =
          Iwram_RatioMulQ14(gCameraSceneParameters.focal, diagonal);
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
