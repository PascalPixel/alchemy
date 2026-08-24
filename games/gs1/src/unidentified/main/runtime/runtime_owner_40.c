#include "types.h"

extern s16 Data_08037206[];
extern s16 Data_08037216[];
s32 GameFlag_IsSet(s32);

s32 Func_08021360(u32 index)
{
    if (index > 8) {
        return 0;
    }
    if (GameFlag_IsSet(32) == 0) {
        return Data_08037206[index];
    }
    return Data_08037216[index];
}
