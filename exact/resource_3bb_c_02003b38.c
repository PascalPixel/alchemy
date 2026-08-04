#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02003b38(s16 arg0) {
    M2C_FIELD(*(void **)0x03001F3C, s16 *, 0xDC) = arg0;
}
