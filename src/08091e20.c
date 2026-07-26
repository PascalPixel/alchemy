#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080030f8(s32);

void Func_08091e20(void) {
    Func_080030f8(M2C_FIELD(*(void **)0x03001EBC, s32 *, 0x1C8));
}
