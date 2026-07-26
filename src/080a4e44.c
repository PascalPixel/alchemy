#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080a23f4(s32, s32, s32, s32, s32);

void Func_080a4e44(void) {
    Func_080a23f4(M2C_FIELD(*(void **)0x03001F2C, s32 *, 0x20), 0xD, 3, 0x11, 0xA);
}
