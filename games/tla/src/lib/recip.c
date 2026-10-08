#include "types.h"

s16 FixedPoint_Reciprocal(s16 value)
{
    return 0x10000 / value;
}
