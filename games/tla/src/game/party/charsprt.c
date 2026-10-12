#include "types.h"

extern const s16 Data_0805f666[];

s32 Func_08044b80(u32 index)
{
    if (index > 8)
        return 0;
    return Data_0805f666[index];
}
