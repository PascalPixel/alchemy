#ifndef ALCHEMY_AFFINE_H
#define ALCHEMY_AFFINE_H

/// @file
/// Sprite rotation and scale, and the affine matrices handed out each frame.

#include "module/runtime/runtime.h"
#include "types.h"
#include "static_assert.h"

/// A sprite's rotation and scale: scales in 8.8 fixed point (0x100 is full
/// size), the angle in 1/65536 turns.
struct AffineTransform {
  u16 scaleX;  ///< 8.8 fixed point, read signed: negative mirrors the sprite
  u16 scaleY;  ///< 8.8 fixed point, read signed
  u16 angle;   ///< 1/65536 turns
  u16 padding; ///< never read; keeps the struct 8 bytes
};

/// One OAM rotation-scaling matrix: pa, pb, pc and pd in 8.8 fixed point, the
/// inverse of the transform the sprite is drawn with.
union OamMatrix {
  s16 coefficients[4]; ///< pa, pb, pc and pd
  /// pa and pb, then pc and pd, written whole when only the diagonal is set.
  u32 rows[2];
};

STATIC_ASSERT(sizeof(struct AffineTransform) == 8);
STATIC_ASSERT(offsetof(struct AffineTransform, scaleY) == 2);
STATIC_ASSERT(offsetof(struct AffineTransform, angle) == 4);
STATIC_ASSERT(sizeof(union OamMatrix) == 8);

/// Matrices handed out this frame (OamMatrix_AllocFromTransform), up to 32;
/// Oam_ResetFrame sets it back to 0 after each VBlank.
extern u8 gOamMatrixCount;
/// This frame's 32 matrices, copied into OAM with the sprites.
extern union OamMatrix gOamMatrices[];

#endif
