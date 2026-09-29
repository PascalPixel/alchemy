#ifndef ALCHEMY_TRANSFORM_H
#define ALCHEMY_TRANSFORM_H

#include "TYPES.H"

/* The resident runtime's current transform (SYSTEM/RUNTIME_MATRIX.S): three
   rows of 16.16 coefficients, each followed by its translation. */
extern s32 gTransform[12];

/* The stack the current transform is saved on: how many are saved, and the
   next free 48-byte slot of the block Render_ResetTransformState allocates. */
extern s32 gTransformStackDepth;
extern u8 *gTransformStackTop;

#endif
