#include "resource_3cc.h"

#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_0200012c(s32);
void Func_0200012e(s32, s32, s32);

void Func_02000064(void) {
    Func_0200012e(9, 1, 0);
    Func_0200012c(0x44);
}
