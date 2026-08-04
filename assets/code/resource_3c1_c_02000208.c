#include "types.h"

extern s32 Func_020004a6(s32);

s32 Func_02000208(void)
{
    if (Func_020004a6(0x96F) != 0) {
        return 0x02008758;
    }
    return 0x02008614;
}
