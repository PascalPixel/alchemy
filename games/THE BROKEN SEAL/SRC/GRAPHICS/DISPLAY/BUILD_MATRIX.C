#include "TYPES.H"

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

s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
s32 Math_Div(s32 numerator, s32 denominator);

extern u8 gObjAffineCount;
extern union AffineMatrix gObjAffineMatrices[];

s32 AffineMatrix_BuildForEffect(struct Effect *source)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    s32 x_scale;
    s32 y_scale;
    s32 angle;
    u8 index;

    index = gObjAffineCount;
    x_scale = (s16)source->x;
    y_scale = (s16)source->y;
    angle = source->angle;
    if (index > 31)
        return 0;

    matrix = &gObjAffineMatrices[index];
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

        sine = Trig_Sin(angle);
        cosine = Trig_Cos(angle);
        *coefficient = Math_Div(cosine, x_scale);
        coefficient++;
        *coefficient = Math_Div(sine, x_scale);
        coefficient++;
        *coefficient = Math_Div(-sine, y_scale);
        coefficient++;
        *coefficient = Math_Div(cosine, y_scale);
    }

    gObjAffineCount = index + 1;
    return index;
}
