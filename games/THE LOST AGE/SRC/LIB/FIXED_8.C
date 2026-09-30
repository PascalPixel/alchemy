#include "TYPES.H"

s32 Math_Div(s32, s32);

s16 Math_ScaleByRatio(s16 arg0, s16 arg1)
{
    return Math_Div(arg0 << 8, arg1);
}
