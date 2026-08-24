#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_020023f0(s32, s32);

void Func_02000040(void) {
    Func_020023f0(0xB, 0x3E);
}
