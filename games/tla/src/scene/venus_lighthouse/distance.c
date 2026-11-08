#include "types.h"
#include "iwram_call.h"

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 dxsq = dx * dx;
  s32 dysq = dy * dy;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dysq + dzsq);
}
