#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_02002a02(s32, s32);

s32 Func_02000030(s32 arg0) {
    if (*(s32 *)0x0200B698 != 0) {
        Func_02002a02(arg0, 2);
        *(s32 *)0x0200B698 = 0;
    }
    return 1;
}
#include "resource_38f.h"
