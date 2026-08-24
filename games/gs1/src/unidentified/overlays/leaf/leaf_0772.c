#include "resource_3cc.h"

#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02000114(s32);
void Func_02000116(s32, s32, s32);

void Func_0200004c(void) {
    Func_02000116(8, 0, 0);
    Func_02000114(0x30);
}
