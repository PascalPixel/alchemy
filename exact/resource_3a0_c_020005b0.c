#include "types.h"

extern u8 Data_02009900[];
extern s32 Func_02001956(s32 flagId);

s32 Func_020005b0(void)
{
    if (Func_02001956(0x895) != 0)
        Data_02009900[0xbe] = 0;
    return (s32)Data_02009900;
}
