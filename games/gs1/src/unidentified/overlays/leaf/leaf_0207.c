#include "resource_3cc.h"

#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02000144(s32);
void Func_02000146(s32, s32, s32);

void Func_0200007c(void) {
    Func_02000146(0xA, 2, 0);
    Func_02000144(0x58);
}
