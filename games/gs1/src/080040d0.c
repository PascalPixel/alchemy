#include "types.h"

extern u16 Data_03001b10[];
s32 Resource_CopyData(s32, u16, s32);

s32 Resource_GetBuffer(s32 index, s32 value)
{
    return Resource_CopyData(index, Data_03001b10[index * 2], value);
}
