#ifndef ALCHEMY_AFFINE_H
#define ALCHEMY_AFFINE_H

/// @file
/// Sprite rotation and scaling: a sprite's transform and the OAM matrices this
/// frame hands out for it.

#include "types.h"
#include "static_assert.h"

/// Full size in an AffineTransform's 8.8 scales.
#define AFFINE_SCALE_ONE (1 << 8)

/// A sprite's rotation and scale: scales in 8.8 fixed point (AFFINE_SCALE_ONE
/// is full size), the angle in 1/65536 turns.
struct AffineTransform {
  u16 scaleX;  ///< 8.8 fixed point, read signed
  u16 scaleY;  ///< 8.8 fixed point, read signed
  u16 angle;   ///< 1/65536 turns
  u16 padding; ///< never read; keeps the struct 8 bytes
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

/// Matrices handed out this frame (OamMatrix_AllocFromTransform), up to 32;
/// Oam_ResetFrame sets it back to 0 after each VBlank.
extern u8 gOamMatrixCount;
/// This frame's 32 matrices, copied into OAM with the sprites.
extern union AffineMatrix gOamMatrices[];

s32 OamMatrix_AllocFromTransform(struct AffineTransform *source);
s32 AffineMatrix_Build(s32 angle, s32 xScale, s32 yScale);

#endif
