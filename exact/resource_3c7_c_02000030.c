#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u8 Func_020012de(s32, s32);

s32 Func_02000030(void) {
    Func_020012de(0xE, 0x102);
    return 0;
}
