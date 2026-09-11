#include "types.h"

s32 BattleFx_GetFlags();

s32 BattleFx_FilterObjectIdByFlags(s32 arg0)
{
    if (BattleFx_GetFlags() == 0xFF) {
        return -1;
    }
    return arg0;
}
