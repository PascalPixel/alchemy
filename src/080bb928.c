#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080bb928(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x16C) = (s32) (M2C_FIELD(arg0, s32 *, 0x16C) | 1);
}
