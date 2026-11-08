#include "types.h"
#include "iwram_call.h"
#include "fixed_math.h"

extern u32 gCameraSceneParameters[];

struct ScanlineRow {
  s32 scale;
  s32 depth;
  s32 offsetX;
  s32 offsetY;
  s32 unk10;
};

// An inline function over the fixed address, as ☀️'s iwram_call.h reaches
// this entry (☀️'s returns the entry's s32).
#undef Iwram_TransformVector
static inline void Iwram_TransformVector(const void *vector, s32 *result) {
  Iwram_TransformVectorEntry(vector, result);
}

void Func_0802dd70(s32 pitchSlope, s32 *position, struct ScanlineRow *row) {
  s32 vector[3];
  s32 result[3];
  s32 height;
  s32 nearZ;
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
  /* FAKEMATCH(wrapper): the call through this file's inline; as the direct
   * Iwram_TransformVector macro call, 41 of 132 instructions differ in
   * tla-en. */
  Iwram_TransformVector(vector, result);
  height = result[1] - Iwram_MulQ16(result[2], pitchSlope);
  nearZ = -gCameraSceneParameters[0];
  for (i = 0; i < 160; i++) {
    s32 distance;

    z = Iwram_RatioMulQ14(nearZ, (gCameraSceneParameters[4] - i) * Q16_ONE);
    distance = z - pitchSlope;
    if (distance == 0)
      distance = 1;
    ratio = Iwram_RatioMulQ14(distance, height);
    if (ratio < 0) {
      row->scale = Iwram_RatioMulQ14(gCameraSceneParameters[0],
                                     Iwram_MulQ16(-ratio, Q16_ONE / 2));
      z = Iwram_MulQ16(ratio, z);
      dx = (result[2] - ratio) >> 4;
      z -= result[1];
      sum = Iwram_MulQ16(dx, dx);
      z >>= 4;
      sum += Iwram_MulQ16(z, z);
      {
        s32 length = Iwram_Sqrt(sum) << 12;

        if (z < 0)
          length = -length;
        row->depth = Iwram_MulQ16(length, Q16_ONE / 2);
      }
    } else {
      row->scale = 0;
      row->depth = 0;
    }
    row->offsetX = 0;
    row->offsetY = 0;
    row++;
  }
}
