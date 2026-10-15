#ifndef ALCHEMY_AFFINE_H
#define ALCHEMY_AFFINE_H

#include "unit/runtime/runtime.h"
#include "types.h"
#include "layout_guard.h"

// A sprite's rotation and scale: scales in 8.8 fixed point (0x100 is full
// size), the angle in 1/65536 turns.
struct AffineTransform {
  u16 scaleX;
  u16 scaleY;
  u16 angle;
  u16 padding; // never read; keeps the struct 8 bytes
};

// One OAM rotation-scaling matrix: pa, pb, pc and pd in 8.8 fixed point, the
// inverse of the transform the sprite is drawn with.
union AffineMatrix {
  s16 coefficients[4];
  u32 rows[2];
};

LAYOUT_SIZE_GUARD(AffineTransform_Size, struct AffineTransform, 8);
LAYOUT_OFFSET_GUARD(AffineTransform_ScaleY, struct AffineTransform, scaleY, 2);
LAYOUT_OFFSET_GUARD(AffineTransform_Angle, struct AffineTransform, angle, 4);
LAYOUT_SIZE_GUARD(AffineMatrix_Size, union AffineMatrix, 8);

// The matrices handed out this frame (up to 32), copied into OAM with the
// sprites
extern u8 gObjAffineCount;
extern union AffineMatrix gObjAffineMatrices[];

#endif
