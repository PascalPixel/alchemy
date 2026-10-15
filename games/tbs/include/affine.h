#ifndef ALCHEMY_AFFINE_H
#define ALCHEMY_AFFINE_H

#include "unit/runtime/runtime.h"
#include "types.h"
#include "layout_guard.h"

struct AffineTransform
{
    u16 scaleX;
    u16 scaleY;
    u16 angle;
    u16 unk06;
};

union AffineMatrix
{
    s16 coefficients[4];
    u32 rows[2];
};

LAYOUT_SIZE_GUARD(AffineTransform_Size, struct AffineTransform, 8);
LAYOUT_OFFSET_GUARD(AffineTransform_ScaleY, struct AffineTransform, scaleY, 2);
LAYOUT_OFFSET_GUARD(AffineTransform_Angle, struct AffineTransform, angle, 4);
LAYOUT_SIZE_GUARD(AffineMatrix_Size, union AffineMatrix, 8);

extern u8                 gObjAffineCount;
extern union AffineMatrix gObjAffineMatrices[];

#endif
