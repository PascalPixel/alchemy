#ifndef ALCHEMY_AFFINE_H
#define ALCHEMY_AFFINE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* Object scale is stored as halfwords and interpreted signed by the matrix
   builder. The angle is unsigned; the final halfword has no known use. */
struct AffineTransform {
    u16 scale_x;
    u16 scale_y;
    u16 angle;
    u16 unknown_06;
};

union AffineMatrix {
    s16 coefficients[4];
    u32 rows[2];
};

LAYOUT_SIZE_GUARD(AffineTransform_Size, struct AffineTransform, 8);
LAYOUT_OFFSET_GUARD(AffineTransform_ScaleY, struct AffineTransform, scale_y, 2);
LAYOUT_OFFSET_GUARD(AffineTransform_Angle, struct AffineTransform, angle, 4);
LAYOUT_SIZE_GUARD(AffineMatrix_Size, union AffineMatrix, 8);

extern u8 gObjAffineCount;
extern union AffineMatrix gObjAffineMatrices[];
s32 AffineMatrix_BuildForEffect(struct AffineTransform *transform);

#endif
