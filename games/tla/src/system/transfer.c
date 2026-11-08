#include "types.h"
#include "iwram_call.h"
#include "transform.h"

void Transform_SetLookAt(s32 *eye, s32 *target) {
  Transform_BuildLookAt(eye, target, gTransform);
}

void Transform_ApplyLookAt(s32 *eye, s32 *target) {
  s32 work[12];

  Transform_BuildLookAt(eye, target, work);
  Iwram_TransformMatrix(work);
}
