#ifndef ALCHEMY_AFFINE_H
#define ALCHEMY_AFFINE_H

#include "types.h"
#include "static_assert.h"

/// A sprite's rotation and scale: scales in 8.8 fixed point (0x100 is full
/// size), the angle in 1/65536 turns.
struct AffineTransform {
  u16 scaleX; ///< 8.8 fixed point, read signed
  u16 scaleY; ///< 8.8 fixed point, read signed
  u16 angle;  ///< 1/65536 turns
  u16 unk06;  ///< not read by AffineMatrix_BuildForEffect
};

/// One OAM rotation-scaling matrix: pa, pb, pc and pd in 8.8 fixed point, the
/// inverse of the transform the sprite is drawn with.
union AffineMatrix {
  s16 coefficients[4]; ///< pa, pb, pc and pd
  /// pa and pb, then pc and pd, written whole when only the diagonal is set.
  u32 rows[2];
};

STATIC_ASSERT(sizeof(struct AffineTransform) == 8);
STATIC_ASSERT(offsetof(struct AffineTransform, scaleY) == 2);
STATIC_ASSERT(offsetof(struct AffineTransform, angle) == 4);
STATIC_ASSERT(sizeof(union AffineMatrix) == 8);

extern u8 gObjAffineCount;
extern union AffineMatrix gObjAffineMatrices[];

s32 AffineMatrix_BuildForEffect(struct AffineTransform *source);
s32 AffineMatrix_Build(s32 angle, s32 x_scale, s32 y_scale);

#endif
