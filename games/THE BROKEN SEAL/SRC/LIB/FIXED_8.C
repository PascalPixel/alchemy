#include "TYPES.H"
#include "SCENE.H"

s32 FixedPoint_Multiply8_8(s32 arg0, s32 arg1)
{
    s16 left = arg0;
    s16 right = arg1;
    s32 product;
    s32 adjusted;
    s32 multiplier;

    multiplier = right;
    product = left;
    product *= multiplier;
    if (product >= 0) {
        adjusted = product;
    } else {
        adjusted = product + 255;
    }
    return (s16)(adjusted >> 8);
}

s32 Math_Div(s32, s32);

s16 Math_ScaleByRatio(s16 arg0, s16 arg1)
{
    return Math_Div(arg0 << 8, arg1);
}

s16 FixedPoint_Reciprocal(s16 value)
{
    return Math_Div(0x10000, value);
}
