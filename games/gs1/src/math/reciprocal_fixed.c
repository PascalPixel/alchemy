#include "types.h"

s16 FixedPoint_Ratio(s32, s16);

s16 FixedPoint_Reciprocal(s16 arg0) {
    return FixedPoint_Ratio(0x10000, arg0);
}
