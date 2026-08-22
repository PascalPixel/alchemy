#include "types.h"

extern u8 Data_080c7420[];

s32 Func_080c2454(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return Data_080c7420[index * 8 + 4];
}
