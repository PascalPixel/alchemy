#include "TYPES.H"

s32 Func_080915dc();
#define BattleFx_GetFlags Func_080915dc

s32 BattleFx_FilterObjectIdByFlags(s32 arg0)
{
    if (BattleFx_GetFlags() == 0xFF) {
        return -1;
    }
    return arg0;
}
