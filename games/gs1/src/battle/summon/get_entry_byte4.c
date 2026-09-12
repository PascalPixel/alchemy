#include "types.h"

extern u8 Data_080c7420[];

s32 Summon_GetEntryByte4(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return Data_080c7420[index * 8 + 4];
}
