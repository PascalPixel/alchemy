#include "types.h"

extern s32 Func_02001956(s32 flagId);
s32 Func_020005b0(void)
{
    if (Func_02001956(0x895) != 0)
        *(u8 *)(0x02009900 + 0xbe) = 0;
    return 0x02009900;
}
