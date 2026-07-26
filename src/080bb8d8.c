#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080bb8d8(void) {
    M2C_FIELD(*(void **)0x03001EE4, s32 *, 8) = 1;
}
