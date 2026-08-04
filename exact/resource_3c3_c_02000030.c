#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0200099c(s32, s32);

void Func_02000030(void) {
    Func_0200099c(0xD, 0x1A);
}
