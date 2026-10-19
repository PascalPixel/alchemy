#ifndef ALCHEMY_AFFINE_H
#define ALCHEMY_AFFINE_H

#include "types.h"
#include "static_assert.h"

struct AffineTransform {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 unk06;
};

union AffineMatrix {
  s16 coefficients[4];
  u32 rows[2];
};

STATIC_ASSERT(sizeof(struct AffineTransform) == 8);
STATIC_ASSERT(offsetof(struct AffineTransform, scaleY) == 2);
STATIC_ASSERT(offsetof(struct AffineTransform, angle) == 4);
STATIC_ASSERT(sizeof(union AffineMatrix) == 8);

extern u8 gObjAffineCount;
extern union AffineMatrix gObjAffineMatrices[];
s32 AffineMatrix_BuildForEffect(struct AffineTransform *transform);

#endif
