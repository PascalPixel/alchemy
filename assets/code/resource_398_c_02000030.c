#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_02000976(s32, s32);

s32 Func_02000030(s32 arg0) {
    Func_02000976(arg0, 0);
    return 0;
}
