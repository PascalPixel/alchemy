#include "types.h"

s32 FixedPoint_Ratio(s32, s32);

s16 FixedPoint_ScaleByRatio(s16 arg0, s16 arg1) {
    return FixedPoint_Ratio(arg0 << 8, arg1);
}
