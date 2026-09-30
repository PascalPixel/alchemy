#include "TYPES.H"

s32 Math_Div(s32, s32);

s16 FixedPoint_Reciprocal(s16 value)
{
    return Math_Div(0x10000, value);
}
