#include "types.h"

s32 Func_080915dc();

s32 BattleFx_FilterObjectIdByFlags(s32 arg0)
{
    if (Func_080915dc() == 0xFF) {
        return -1;
    }
    return arg0;
}
