#include "resource_3cc.h"

#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0200015c(s32);
void Func_0200015e(s32, s32, s32);

void Func_02000094(void) {
    Func_0200015e(0xB, 3, 0);
    Func_0200015c(0x6C);
}
