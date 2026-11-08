// Matches: differ 0 (tla-en, 2026-11-08)
// The same routine as ☀️'s WorldMap_BuildScanlineTable (games/tbs/src/field/
// field/view.c), over ⚓️'s projection record (gCameraSceneParameters: the
// focal length at [0], the centre line at [4]). For each of the 160
// scanlines, projects a ground point through the camera and stores its width
// scale and signed ground distance in 16.16, or zeros above the horizon.
// What settled it (2026-11-08): the transform reached through an inline
// function, as ☀️'s iwram_call.h reaches every IWRAM entry; the inline's
// parameter keeps the call's argument apart from cameraCoordinates, set
// after the call as in ☀️, so CSE no longer merges the two (as the direct
// macro call, 41 of 132 instructions differ). The vector's zero height
// comes from a local holding 0, as in ☀️. Earlier rounds' pointers to
// result[1] or result[2], and a zero local in the distance test, are gone.
// Before adoption: ⚓️'s iwram_call.h calls this entry directly; the inline
// below is ☀️'s header form and belongs there if other users still match.
#include "types.h"
#include "iwram_call.h"

extern u32 gCameraSceneParameters[];

struct ScanlineRow {
  s32 x;
  s32 y;
  s32 unk08;
  s32 unk0c;
  s32 unk10;
};

// ☀️'s iwram_call.h form of this entry: an inline function over the fixed
// address.
#undef Iwram_TransformVector
static inline void Iwram_TransformVector(const void *vector, s32 *result) {
  Iwram_TransformVectorEntry(vector, result);
}

void Func_0802dd70(s32 depth, s32 *position, struct ScanlineRow *row) {
  s32 vector[3];
  s32 result[3];
  s32 height;
  s32 nearZ;
  s32 *cameraCoordinates;
  s32 i;
  s32 z;
  s32 ratio;
  s32 dx;
  s32 sum;
  s32 zero;

  zero = 0;
  vector[0] = position[0];
  vector[1] = zero;
  vector[2] = position[2];
  Iwram_TransformVector(vector, result);
  cameraCoordinates = result;
  height = cameraCoordinates[1] - Iwram_MulQ16(cameraCoordinates[2], depth);
  nearZ = -gCameraSceneParameters[0];
  for (i = 0; i < 160; i++) {
    s32 distance;

    z = Iwram_RatioMulQ14(nearZ, (gCameraSceneParameters[4] - i) << 16);
    distance = z - depth;
    if (distance == 0)
      distance = 1;
    ratio = Iwram_RatioMulQ14(distance, height);
    if (ratio < 0) {
      row->x = Iwram_RatioMulQ14(gCameraSceneParameters[0],
                                 Iwram_MulQ16(-ratio, 0x8000));
      z = Iwram_MulQ16(ratio, z);
      dx = (cameraCoordinates[2] - ratio) >> 4;
      z -= cameraCoordinates[1];
      sum = Iwram_MulQ16(dx, dx);
      z >>= 4;
      sum += Iwram_MulQ16(z, z);
      {
        s32 length = Iwram_Sqrt(sum) << 12;

        if (z < 0)
          length = -length;
        row->y = Iwram_MulQ16(length, 0x8000);
      }
    } else {
      row->x = 0;
      row->y = 0;
    }
    row->unk08 = 0;
    row->unk0c = 0;
    row++;
  }
}
