#include "types.h"
#include "scene.h"
#include "abi/graphics/affine/build_matrix.h"

typedef s32 (*SignedDivide)(s32 numerator, s32 denominator);

struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

union AffineMatrix {
    s16 coefficients[4];
    u32 rows[2];
};

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);

extern u8 gIw;
extern union AffineMatrix gIw2[];

s32 AffineMatrix_BuildForEffect(struct Effect *source)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    s32 x_scale;
    s32 y_scale;
    s32 angle;
    u8 index;

    index = gIw;
    x_scale = (s16)source->x;
    y_scale = (s16)source->y;
    angle = source->angle;
    if (index > 31)
        return 0;

    matrix = &gIw2[index];
    coefficient = matrix->coefficients;
    if ((x_scale == y_scale || -x_scale == y_scale) && angle == 0) {
        SignedDivide divide;
        s32 reciprocal;
        s32 x_reciprocal;

        divide = (SignedDivide)0x03000380;
        reciprocal = divide(0x10000, y_scale);
        x_reciprocal = reciprocal;
        if (-x_scale == y_scale)
            x_reciprocal = -reciprocal;

        matrix->rows[0] = (u16)x_reciprocal;
        matrix->rows[1] = (u32)reciprocal << 16;
    } else {
        s32 sine;
        s32 cosine;

        sine = Sys_Check(angle);
        cosine = Sys_Check2(angle);
        *coefficient = FixedPoint_Ratio(cosine, x_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(sine, x_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(-sine, y_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(cosine, y_scale);
    }

    gIw = index + 1;
    return index;
}
